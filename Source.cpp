#include "Graph.h"
#include "Drawing.h"
#include <iostream>

//Design todo-s
// +++ TODO #1: implement edge class in drawing.h
// +++ TODO #2: implement removing of vertices. if vertex removed, all her edges removed too
// +++ TODO #3: implement connection of draggingEdge to second vertex if mouse released
// ??? TODO #4: split up drawing.h to drawing.hpp and drawing.cpp
//TODO #5: fix vertex removing(edge have ptrs to its vertices and when vertex is removed ptr may point to another vertex which is obviously incorrect)
//TODO #6: implement menu for input edge weight etc.
//TODO #7: implement deleteVertexFromScreen in O(1)
 
//Performance todo-s
//TODO #1: check memory leaks
//TODO #2: copy constructor for vertex struct(cause it's copied to vector)

using namespace sf;
using namespace tgui;

void onTabSelected(tgui::Gui& gui, std::string selectedTab)
{
	// Show the correct panel
	if (selectedTab == "Graph") gui.get("MenuPanel")->hide();
	else if (selectedTab == "Menu") gui.get("MenuPanel")->show();
}


void init() {
	if (font.loadFromFile("fonts/arial.ttf"))  //загрузить шрифт для надписей
		fontLoaded = true;

	if (arrowTexture.loadFromFile("sprites/arrow.png")) //загрузить спрайт для стрелки на дуге
		arrowTemplate.setTexture(arrowTexture);
		 
	view.reset(sf::FloatRect(0,0,WIDTH,HEIGHT));

	//настройка вкладок для 1)отображения графа и 2)меню
	tabs = tgui::Tab::create();
	tabs->add("Graph");
	tabs->add("Menu");
	tabs->setPosition(0, 0);
	tabs->setSize(0, 0);
	gui.add(tabs);

	// Create the first panel
	tgui::Panel::Ptr menuPanel = tgui::Panel::create();
	menuPanel->setSize(WIDTH, HEIGHT);
	menuPanel->setPosition(tabs->getPosition().x, tabs->getPosition().y + tabs->getTabHeight());
	//добавить виджеты на панель
	menuPanel->add(tgui::Button::create("Ford-Bellman"));
	//добавить панель в gui
	gui.add(menuPanel, "MenuPanel");

	// Enable callback when another tab is selected (pass reference to the gui as first parameter)
	tabs->connect("TabSelected", onTabSelected, std::ref(gui));

	// Select the first tab and only show the first panel
	tabs->select("Graph");
	menuPanel->hide();

	//настройка поля для ввода веса дуги
	inputWeightTextBox = tgui::TextBox::create(); //поле для ввода веса дуги
	inputWeightInProcess = false; //изначально вес дуги не вводится
	inputWeightTextBox->setSize(inputWeightTextBoxSize); //задать размеры этого поля
	inputWeightTextBox->hide(); //скрыть его вначале
	gui.add(inputWeightTextBox); //добавить на экран
}



int main() {
	Graph g;

	init(); //начальная инициализация

	InterfaceObject *collisionPress=nullptr, *collisionRelease=nullptr; //указатели на элемент интерфейса, на которые попали нажатие и отпускание мыши(вершина, ребро, или кнопка)

	bool dragVertex = false; //drag'n'drop для вершины
	bool dragEdge = false; //drag'n'drop для ребра
	float dx = 0, dy = 0; //смещение drag'n'drop относительно координат нажатия мыши
	Vector2f mouseWindowSelectVertexStart; //координаты начального нажатия на вершину
	Vector2i mouseWindowPos; //координаты окна
	Vector2f mouseAppPos; //координаты внутри приложения
	Vector2f mousePressPos; //координаты нажатия мыши(используется для регистрации первого нажатия)

	bool mousePressed = false; //зажата ли мышь в данный момент(чтобы не обновлять collisionPress)
	float mouseTravelledDistance = 0.0f; //расстояние, пройденное мышью

	sf::Vertex draggingEdge[2]; //ребро, которое тянется
	draggingEdge[0].color = edgeColor;
	draggingEdge[1].color = edgeColor;

	edge* selectedEdge = nullptr; //указатель на выделенное ребро

	while (app.isOpen()) {

		mouseWindowPos = Mouse::getPosition(app); //получить координаты окна
		mouseAppPos = app.mapPixelToCoords(mouseWindowPos); //получить координаты приложения(т.к. экран приложения может смещаться за																																пределы окна)

		Event event;
		
		while (app.pollEvent(event)) {
			if (event.type == Event::Closed)
				app.close();

			// Resize window : set new size
			if (event.type == sf::Event::Resized)
				view.setSize(event.size.width,event.size.height); //расширить окно до размера приложения

			if (inputWeightInProcess) selectedEdge->inputWeight(inputWeightTextBox);

			gui.handleEvent(event) == false; //обработка gui событий

			if (tabs->getSelected() == "Graph") {
				//обработка нажатий мыши
				if (event.type == Event::MouseButtonPressed && mouseAppPos.y - (tabs->getPosition().y + tabs->getTabHeight()) > 5 && !mousePressed) { //если еще не нажата
					if (!mousePressed) {
						mousePressed = true;
						mousePressPos = mouseAppPos;
						collisionPress = findCollision(mouseAppPos);
					}
					if (event.mouseButton.button == Mouse::Left)
						if (collisionPress != nullptr) { //если попали на какой-то объект
							if (collisionPress->getType() == "Vertex") {  //если попали на вершину 
								if (collisionPress->isSelected()) { //если она выделена
									dx = mouseAppPos.x - collisionPress->getCoord().x; //вычислить смещение
									dy = mouseAppPos.y - collisionPress->getCoord().y;

									dragVertex = true; //включить drag'n'drop для вершины 

								}
								else { //если не выделена, drag'n'drop для ребра
									draggingEdge[0].position = collisionPress->getCoord();

									dragEdge = true;
								}
							}
						}
				}

				//обработка отпусканий мыши
				if (event.type == Event::MouseButtonReleased && mouseAppPos.y > 50) //если кнопка мыши отпущена
				{
					mousePressed = false;
					collisionRelease = findCollision(mouseAppPos); //определяем, попадает ли нажатие в какой-то объект
					if (event.mouseButton.button == Mouse::Left) //если отпущена ЛКМ
					{
						if (collisionRelease != nullptr) { //если попадание в объект
							if (collisionRelease->getType() == "Vertex") { //если попадание в вершину
								if (dragVertex) {
									dragVertex = false; //если тянули вершину, отменить
								}
								if (dragEdge) { //если тянули ребро, отменить и прикрепить его к другой вершине
									dragEdge = false;
									int indexFirstVertex = ((vertex*)collisionPress)->getIndex(), indexSecondVertex = ((vertex*)collisionRelease)->getIndex(); //получаем индексы вершин данного ребра
									if (indexFirstVertex != indexSecondVertex && !g.connected(indexFirstVertex, indexSecondVertex)) { //если вершины еще не соединены, то соединяем
										edge e((vertex*)collisionPress, (vertex*)collisionRelease, 1);
										addEdgeToScreen(&e);

										g.addEdge(indexFirstVertex, indexSecondVertex, 1);
									}
								}
								if (mouseTravelledDistance < MOUSE_TRAVEL_THRESHOLD) {
									if (collisionRelease->isSelected()) collisionRelease->deselect(); //если она выделена, отменить выделение
									else collisionRelease->select(); //если она не выделена, выделить
								}

								mouseTravelledDistance = 0.0f; //обнуляем расстояние, пройденное мышью
							}
							else if (collisionRelease->getType() == "Edge") {
								if (dragEdge) dragEdge = false;
								else if (collisionRelease->isSelected()) {
									g.setWeight(((edge*)selectedEdge)->getFirstVertex()->getIndex(), ((edge*)selectedEdge)->getSecondVertex()->getIndex(), ((edge*)selectedEdge)->getWeight()); //обновить вес для ребра в структуре данных для хранения графа

									inputWeightTextBox->setText("");

									collisionRelease->deselect(); //если ребро выделено, отменить выделение
									selectedEdge = nullptr; //выделенных ребер теперь нет
									inputWeightTextBox->hide();
									inputWeightInProcess = false;
								}
								else {
									inputWeightTextBox->setText("");

									if (selectedEdge != nullptr) {
										g.setWeight(((edge*)selectedEdge)->getFirstVertex()->getIndex(), ((edge*)selectedEdge)->getSecondVertex()->getIndex(), ((edge*)selectedEdge)->getWeight()); //обновить вес для ребра в структуре данных для хранения графа
										selectedEdge->deselect(); //развыделить предыдущее выделенное ребро
									}
									collisionRelease->select(); //если ребро не выделено, выделить
									selectedEdge = (edge*)(collisionRelease);
									inputWeightInProcess = true;
									selectedEdge->inputWeight(inputWeightTextBox);
								}
							}
						}
						else { //если нет попадания в объект
							if (dragEdge) dragEdge = false; //если тянули ребро, отменяем
							else {
								vertex v(mouseAppPos.x, mouseAppPos.y, g.getMaxIndex()); //добавить новую вершину на экран
								addVertexToScreen(&v);

								g.addVertex(); //добавляем вершину в структуру данных
							}
						}
					}

					if (event.mouseButton.button == sf::Mouse::Right) { //если отпущена ПКМ
						if (collisionRelease != nullptr) {
							if (collisionRelease->getType() == "Vertex") { //если попали на вершину
								g.deleteVertex(((vertex*)(collisionRelease))->getIndex()); //удаляем вершину из структуры данных

								deleteVertexFromScreen((vertex*)(collisionRelease)); //удаляем вершину с экрана
							}
							else if (collisionRelease->getType() == "Edge") {//если попали на ребро
								g.deleteEdge(((edge*)collisionRelease)->getFirstVertex()->getIndex(), ((edge*)collisionRelease)->getSecondVertex()->getIndex());

								deleteEdgeFromScreen((edge*)collisionRelease); //удаляем ребро с экрана
							}
						}
					}
				}

				if (mousePressed) { //если мышь зажата
					float frameMouseDistanceX = mouseAppPos.x - mousePressPos.x,
						frameMouseDistanceY = mouseAppPos.y - mousePressPos.y; //вычисляем расстояния, пройденные мышью по x и по y за один кадр
					mouseTravelledDistance += sqrt(frameMouseDistanceX*frameMouseDistanceX + frameMouseDistanceY * frameMouseDistanceY);
				}

				if (dragVertex) { //если вершина в режиме drag'n'drop
					(((vertex*)collisionPress)->move(mouseAppPos.x - dx, mouseAppPos.y - dy)); //переместить ее с учетом смещения
				}
				else if (dragEdge) { //если тянем ребро
					draggingEdge[1].position = mouseAppPos; //перемещаем его конец к мыши
				}
			}

			g.print(); std::cout << endl;
		}

		app.setView(view); //установить параметры окна для приложения(ширина, высота) - используется при переходе в fullscreen например

		app.clear(backg); //очистить окно
		drawGraph(); //отобразить граф

		if (dragEdge) app.draw(draggingEdge, 2, sf::Lines); //отобразить тянущееся ребро

		gui.draw();
		app.display(); //отобразить новый фрейм
	}

	return 0;
}
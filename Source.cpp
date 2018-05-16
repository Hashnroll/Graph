#include "Graph.h"
#include "Drawing.h"
#include <SFML\Graphics.hpp>
#include <iostream>

//Design todo-s
//TODO #1: make a button for connecting vertices
//TODO #2: implement function "orderVerticesIndices" cause after removing some vertices their indices are "lost"

//Performance todo-s
//TODO #1: check memory leaks
//TODO #2: copy constructor for vertex struct(cause it's copied to vector)

using namespace sf;

void init() {
	vertices.reserve(MAX_VERTICES); //установить макс. кол-во вершин

	if (vertexNameFont.loadFromFile("fonts/arial.ttf")) 
		vertexNameFontLoaded = true;
		 
}

int main() {
	Graph g;


	init(); //начальная инициализация

	vector<vertex>::iterator collision; //итератор, указывающий на нажатие мыши
	bool dragVertex = false; //для drag'n'drop на вершины
	float dx = 0, dy = 0; //смещение drag'n'drop относительно координат нажатия мыши

	while (app.isOpen()) {
		Vector2i mouseWindowPos = Mouse::getPosition(app); //получить координаты окна
		Vector2f mouseAppPos = app.mapPixelToCoords(mouseWindowPos); //получить координаты приложения(т.к. экран приложения может смещаться за																																пределы окна)

		Event event;

		while (app.pollEvent(event)) {
			if (event.type == Event::Closed)
				app.close();

			//обработка нажатий мыши, по нажатию происходит только drag'n'drop
			if (event.type == Event::MouseButtonPressed) { 
				collision = findCollision(event.mouseButton);
				if (event.mouseButton.button == Mouse::Left)
					if (collision != vertices.end()) {
						dragVertex = true;
						dx = mouseAppPos.x - collision->circle->getPosition().x;
						dy = mouseAppPos.y - collision->circle->getPosition().y;
					}
			}

			//обработка отпусканий мыши, по отпусканиям происходит добавление/удаление
			if (event.type == Event::MouseButtonReleased) 
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					if (collision == vertices.end()) { //отпускание не попало на вершину
						vertex* v = new vertex(event.mouseButton.x, event.mouseButton.y, g.getMaxIndex());
						vertices.push_back(*v);
						g.addVertex();
					}
					else { //отпускание попало на вершину
						dragVertex = false; //выключить режим drag'n'drop

						if (collision->selected == true) { //если вершина активирована, деактивировать только ее
							collision->deselect();
						}
						else { //если вершина деактивирована, активировать ее
							if (!Keyboard::isKeyPressed(Keyboard::LControl)) { //а все остальные деактивировать, если не зажат Left Ctrl
								vector<vertex>::iterator it = vertices.begin();
								while (it != vertices.end()) {
									if (it->selected == true) it->deselect();
									it++;
								}
							}
							collision->select();
						}
					}
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					if (collision != vertices.end()) {
						g.deleteVertex(collision->index);
						vertices.erase(collision); //TODO: это не очищает память процесса; разобраться, как ее очистить
					}
				}
			}

			//обработка нажатий клавиатуры
			if (event.type == Event::KeyPressed) 
			{
				if (event.key.code == Keyboard::Key::Space) {
					for (auto v : vertices) {
						if (v.selected) {
							for (auto w : vertices) {
								if (v.index != w.index && w.selected) { //v!=w так петель нет
									if (!g.connected(v.index, w.index)) //проверяем, соединены ли вершины
										g.addEdge(v.index, w.index);
								}
							}
						}
					}
					connectSelectedVertices(); //соединить графически
				}
			}

			if (dragVertex) { //если вершина в режиме drag'n'drop
				collision->move(mouseAppPos.x - dx, mouseAppPos.y - dy); //переместить ее с учетом смещения
			}
			g.print(); cout << endl;
		}

		app.clear(backg);

		//drawGrid();
		drawGraph(); //отобразить граф
		
		app.display(); 
	}

	return 0;
}
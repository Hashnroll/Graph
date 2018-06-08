#include "Graph.h"
#include "Drawing.h"
#include <SFML\Graphics.hpp>
#include <iostream>

//Design todo-s
// +++ TODO #1: implement edge class in drawing.h
// +++ TODO #2: implement removing of vertices. if vertex removed, all her edges removed too
// +++ TODO #3: implement connection of draggingEdge to second vertex if mouse released
//TODO #4: split up drawing.h to drawing.hpp and drawing.cpp
//TODO #5: fix vertex removing(edge have ptrs to its vertices and when vertex is removed ptr may point to another vertex which is obviously incorrect)
//TODO #6: implement menu for input edge weight etc.
 
//Performance todo-s
//TODO #1: check memory leaks
//TODO #2: copy constructor for vertex struct(cause it's copied to vector)

using namespace sf;

void init() {
	vertices.reserve(MAX_VERTICES); //���������� ����. ���-�� ������

	if (font.loadFromFile("fonts/arial.ttf"))  //��������� ����� ��� ��������
		fontLoaded = true;

	if (arrowTexture.loadFromFile("sprites/arrow.png")) //��������� ������ ��� ������� �� ����
		arrowTemplate.setTexture(arrowTexture);
		 
	view.reset(sf::FloatRect(0,0,WIDTH,HEIGHT));
}

int main() {
	Graph g;

	init(); //��������� �������������

	InterfaceObject *collisionPress = nullptr, *collisionRelease = nullptr; //��������� �� ������� ����������, �� ������� ������ ������� � ���������� ����(�������, �����, ��� ������)
	bool dragVertex = false; //drag'n'drop ��� �������
	bool dragEdge = false; //drag'n'drop ��� �����
	float dx = 0, dy = 0; //�������� drag'n'drop ������������ ��������� ������� ����
	Vector2f mouseWindowSelectVertexStart; //���������� ���������� ������� �� �������
	Vector2i mouseWindowPos; //���������� ����
	Vector2f mouseAppPos; //���������� ������ ����������
	Vector2f mousePressPos; //���������� ������� ����(������������ ��� ����������� ������� �������)

	bool mousePressed = false; //������ �� ���� � ������ ������(����� �� ��������� collisionPress)
	float mouseTravelledDistance = 0.0f; //����������, ���������� �����

	sf::Vertex draggingEdge[2]; //�����, ������� �������
	draggingEdge[0].color = edgeColor;
	draggingEdge[1].color = edgeColor;

	while (app.isOpen()) {

		mouseWindowPos = Mouse::getPosition(app); //�������� ���������� ����
		mouseAppPos = app.mapPixelToCoords(mouseWindowPos); //�������� ���������� ����������(�.�. ����� ���������� ����� ��������� ��																																������� ����)
		//cout << "Window: " << mouseWindowPos.x << " " << mouseWindowPos.y << endl << "App: " << mouseAppPos.x << " " << mouseAppPos.y << endl << endl;

		Event event;

		while (app.pollEvent(event)) {
			if (event.type == Event::Closed)
				app.close();

			// Resize window : set new size
			if (event.type == sf::Event::Resized)
				view.setSize(event.size.width,event.size.height);

			//��������� ������� ����
			if (event.type == Event::MouseButtonPressed && !mousePressed) { //���� ��� �� ������
				if (!mousePressed) {
					mousePressed = true;
					mousePressPos = mouseAppPos;
					collisionPress = findCollision(mouseAppPos);
				}
				if (event.mouseButton.button == Mouse::Left)
					if (collisionPress != nullptr) { //���� ������ �� �����-�� ������
						if (collisionPress->getType() == "Vertex") {  //���� ������ �� ������� 
							if (collisionPress->isSelected()) { //���� ��� ��������
								dx = mouseAppPos.x - collisionPress->getCoord().x; //��������� ��������
								dy = mouseAppPos.y - collisionPress->getCoord().y;

								dragVertex = true; //�������� drag'n'drop ��� ������� 

							}
							else { //���� �� ��������, drag'n'drop ��� �����
								draggingEdge[0].position = collisionPress->getCoord();

								dragEdge = true;
							}
						}
					}
			}

			//��������� ���������� ����
			if (event.type == Event::MouseButtonReleased) //���� ������ ���� ��������
			{
				mousePressed = false;
				collisionRelease = findCollision(mouseAppPos); //����������, �������� �� ������� � �����-�� ������
				if (event.mouseButton.button == Mouse::Left) //���� �������� ���
				{
					if (collisionRelease != nullptr) { //���� ��������� � ������
						if (collisionRelease->getType() == "Vertex") { //���� ��������� � �������
							if (dragVertex) {
								dragVertex = false; //���� ������ �������, ��������
							}
							if (dragEdge) { //���� ������ �����, �������� � ���������� ��� � ������ �������
								dragEdge = false;
								int indexFirstVertex = ((vertex*)collisionPress)->getIndex(), indexSecondVertex = ((vertex*)collisionRelease)->getIndex(); //�������� ������� ������ ������� �����
								if (indexFirstVertex!=indexSecondVertex && !g.connected(indexFirstVertex, indexSecondVertex)) { //���� ������� ��� �� ���������, �� ���������
									edge e((vertex*)collisionPress, (vertex*)collisionRelease, 1);
									addEdgeToScreen(&e);

									g.addEdge(indexFirstVertex, indexSecondVertex,1);
								}
							}
							if (mouseTravelledDistance < MOUSE_TRAVEL_THRESHOLD) {
								if (collisionRelease->isSelected()) collisionRelease->deselect(); //���� ��� ��������, �������� ���������
								else collisionRelease->select(); //���� ��� �� ��������, ��������
							}

							mouseTravelledDistance = 0.0f; //�������� ����������, ���������� �����
						}
						else if (collisionRelease->getType() == "Edge") {
							if (collisionRelease->isSelected()) collisionRelease->deselect(); //���� ����� ��������, �������� ���������
							else collisionRelease->select(); //���� ����� �� ��������, ��������
						}
					}
					else { //���� ��� ��������� � ������
						if (dragEdge) dragEdge = false; //���� ������ �����, ��������
						else {
							vertex v(mouseAppPos.x, mouseAppPos.y, g.getMaxIndex()); //�������� ����� ������� �� �����
							addVertexToScreen(&v);

							g.addVertex(); //��������� ������� � ��������� ������
						}
					}
				}

				if (event.mouseButton.button == sf::Mouse::Right) { //���� �������� ���
					if (collisionRelease != nullptr) {
						if (collisionRelease->getType() == "Vertex") { //���� ������ �� �������
							g.deleteVertex(((vertex*)collisionRelease)->getIndex()); //������� ������� �� ��������� ������

							deleteVertexFromScreen((vertex*)collisionRelease); //������� ������� � ������
						}
						if (collisionRelease->getType() == "Edge") {//���� ������ �� �����
							g.deleteEdge(((edge*)collisionRelease)->getFirstVertex()->getIndex(), ((edge*)collisionRelease)->getSecondVertex()->getIndex());

							deleteEdgeFromScreen((edge*)collisionRelease); //������� ����� � ������
						}
					}
				}
			}

			if (mousePressed) { //���� ���� ������
				float frameMouseDistanceX = mouseAppPos.x - mousePressPos.x,
					frameMouseDistanceY = mouseAppPos.y - mousePressPos.y; //��������� ����������, ���������� ����� �� x � �� y �� ���� ����
				mouseTravelledDistance += sqrt(frameMouseDistanceX*frameMouseDistanceX + frameMouseDistanceY * frameMouseDistanceY);
			}

			if (dragVertex) { //���� ������� � ������ drag'n'drop
				((vertex*)collisionPress)->move(mouseAppPos.x - dx, mouseAppPos.y - dy); //����������� �� � ������ ��������
			}
			else if (dragEdge) { //���� ����� �����
				draggingEdge[1].position = mouseAppPos; //���������� ��� ����� � ����
			}

			g.print(); cout << endl;
		}

		app.setView(view);

		app.clear(backg);

		//drawGrid();
		drawGraph(); //���������� ����

		if (dragEdge) app.draw(draggingEdge, 2, sf::Lines); //���������� ��������� �����

		app.display();
	}

	return 0;
}
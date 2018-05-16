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
	vertices.reserve(MAX_VERTICES); //���������� ����. ���-�� ������

	if (vertexNameFont.loadFromFile("fonts/arial.ttf")) 
		vertexNameFontLoaded = true;
		 
}

int main() {
	Graph g;


	init(); //��������� �������������

	vector<vertex>::iterator collision; //��������, ����������� �� ������� ����
	bool dragVertex = false; //��� drag'n'drop �� �������
	float dx = 0, dy = 0; //�������� drag'n'drop ������������ ��������� ������� ����

	while (app.isOpen()) {
		Vector2i mouseWindowPos = Mouse::getPosition(app); //�������� ���������� ����
		Vector2f mouseAppPos = app.mapPixelToCoords(mouseWindowPos); //�������� ���������� ����������(�.�. ����� ���������� ����� ��������� ��																																������� ����)

		Event event;

		while (app.pollEvent(event)) {
			if (event.type == Event::Closed)
				app.close();

			//��������� ������� ����, �� ������� ���������� ������ drag'n'drop
			if (event.type == Event::MouseButtonPressed) { 
				collision = findCollision(event.mouseButton);
				if (event.mouseButton.button == Mouse::Left)
					if (collision != vertices.end()) {
						dragVertex = true;
						dx = mouseAppPos.x - collision->circle->getPosition().x;
						dy = mouseAppPos.y - collision->circle->getPosition().y;
					}
			}

			//��������� ���������� ����, �� ����������� ���������� ����������/��������
			if (event.type == Event::MouseButtonReleased) 
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					if (collision == vertices.end()) { //���������� �� ������ �� �������
						vertex* v = new vertex(event.mouseButton.x, event.mouseButton.y, g.getMaxIndex());
						vertices.push_back(*v);
						g.addVertex();
					}
					else { //���������� ������ �� �������
						dragVertex = false; //��������� ����� drag'n'drop

						if (collision->selected == true) { //���� ������� ������������, �������������� ������ ��
							collision->deselect();
						}
						else { //���� ������� ��������������, ������������ ��
							if (!Keyboard::isKeyPressed(Keyboard::LControl)) { //� ��� ��������� ��������������, ���� �� ����� Left Ctrl
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
						vertices.erase(collision); //TODO: ��� �� ������� ������ ��������; �����������, ��� �� ��������
					}
				}
			}

			//��������� ������� ����������
			if (event.type == Event::KeyPressed) 
			{
				if (event.key.code == Keyboard::Key::Space) {
					for (auto v : vertices) {
						if (v.selected) {
							for (auto w : vertices) {
								if (v.index != w.index && w.selected) { //v!=w ��� ������ ���
									if (!g.connected(v.index, w.index)) //���������, ��������� �� �������
										g.addEdge(v.index, w.index);
								}
							}
						}
					}
					connectSelectedVertices(); //��������� ����������
				}
			}

			if (dragVertex) { //���� ������� � ������ drag'n'drop
				collision->move(mouseAppPos.x - dx, mouseAppPos.y - dy); //����������� �� � ������ ��������
			}
			g.print(); cout << endl;
		}

		app.clear(backg);

		//drawGrid();
		drawGraph(); //���������� ����
		
		app.display(); 
	}

	return 0;
}
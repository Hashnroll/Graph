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

	while (app.isOpen()) {
		Event event;

		while (app.pollEvent(event)) {
			if (event.type == Event::Closed)
				app.close();
			
			if (event.type == sf::Event::MouseButtonPressed)
			{
				vector<vertex>::iterator collision = findCollision(event.mouseButton);
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (collision == vertices.end()) { //������� �� ������ �� �������
						vertex* v = new vertex(event.mouseButton.x, event.mouseButton.y, g.getMaxIndex());
						vertices.push_back(*v);
						g.addVertex();
					}
					else { //������� ������ �� �������
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
			g.print(); cout << endl;
		}

		app.clear(backg);

		//drawGrid();
		drawGraph(); //���������� ����
		
		app.display(); 
	}

	return 0;
}
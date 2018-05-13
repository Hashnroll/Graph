#include "Graph.h"
#include "Drawing.h"
#include <SFML\Graphics.hpp>

//TODO #1: check memory leaks
//TODO #2: copy constructor for vertex struct(cause it's copied to vector)

using namespace sf;

void init() {
	vertices.reserve(MAX_VERTICES);
}

int main() {
	Graph g;

	init(); //начальная инициализация

	while (app.isOpen()) {
		Event event;

		while (app.pollEvent(event)) {
			if (event.type == Event::Closed)
				app.close();
			
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					vertex* collision = findCollision(event.mouseButton);
					if (collision == NULL) { //нажатие не попало на вершину
						/*CircleShape newVertex(VERTEX_RADIUS);
						newVertex.setOrigin(VERTEX_RADIUS, VERTEX_RADIUS);
						newVertex.setPosition(event.mouseButton.x, event.mouseButton.y);*/
						vertex* v = new vertex(event.mouseButton.x, event.mouseButton.y);
						vertices.push_back(*v);
						//
						g.addVertex();
					}
					else { //нажатие попало на вершину
						if (collision->circle->getFillColor()==vertexColor) collision->circle->setFillColor(vertexColorSelected);
						else collision->circle->setFillColor(vertexColor);
					}
				}
			}
		}

		app.clear(backg);

		//drawGrid();
		drawGraph(); //отобразить граф
		
		app.display(); 
	}

	return 0;
}
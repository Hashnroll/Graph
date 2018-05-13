#include "Graph.h"
#include "Drawing.h"
#include <SFML\Graphics.hpp>

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
					CircleShape newVertex(VERTEX_RADIUS);
					newVertex.setOrigin(VERTEX_RADIUS, VERTEX_RADIUS);
					newVertex.setPosition(event.mouseButton.x, event.mouseButton.y);
					vertices.push_back(newVertex);
				}
			}
		}

		app.clear(backg);

		//drawGrid();
		drawGraph();
		
		app.display(); 
	}

	return 0;
}
#pragma once
//#include <SFML\Graphics.hpp>
#include "Settings.h"
#include <vector>

struct vertex {
	sf::CircleShape* circle;
	sf::FloatRect bound;

	vertex(int x, int y) {
		circle = new sf::CircleShape(VERTEX_RADIUS);
		circle->setOrigin(VERTEX_RADIUS, VERTEX_RADIUS);
		circle->setPosition(x, y);
		circle->setFillColor(vertexColor);

		bound = circle->getLocalBounds();
		bound.left = circle->getPosition().x - 3*VERTEX_RADIUS;
		bound.top = circle->getPosition().y - 3*VERTEX_RADIUS;
		bound.width = bound.height = 6 * VERTEX_RADIUS;
	}

	sf::CircleShape getCircle() {
		return *circle;
	}

	/*~vertex() {
		delete circle;
	}*/
};

std::vector<vertex> vertices;

void drawGraph() {
	for (auto v : vertices) {
		app.draw(v.getCircle());
	}
}

vertex* findCollision(sf::Event::MouseButtonEvent press) {
	for (auto v : vertices) {
		if (v.bound.contains(sf::Vector2f(press.x, press.y))) return &v;
	}
	return NULL;
}

void drawGrid() {

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};

	//app.draw(line, 2, sf::Lines);
}
#pragma once
//#include <SFML\Graphics.hpp>
#include "Settings.h"
#include <vector>

std::vector<sf::CircleShape> vertices;

void drawGraph() {
	for (auto v : vertices) {
		v.setFillColor(vertexColor);
		app.draw(v);
	}
}

void drawGrid() {

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};

	//app.draw(line, 2, sf::Lines);
}
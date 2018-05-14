#pragma once
//#include <SFML\Graphics.hpp>
#include "Settings.h"
#include <vector>
#include <iterator>
#include <string>

struct vertex {
	sf::CircleShape* circle;
	sf::FloatRect bound;
	bool selected;

	int index;
	sf::Text name;

	vertex(int x, int y, int ind) {
		circle = new sf::CircleShape(VERTEX_RADIUS); //настройка круга
		circle->setOrigin(VERTEX_RADIUS, VERTEX_RADIUS);
		circle->setPosition(x, y);
		circle->setFillColor(vertexColor);

		bound = circle->getLocalBounds(); //настройка границы
		bound.left = circle->getPosition().x - 2*VERTEX_RADIUS;
		bound.top = circle->getPosition().y - 2*VERTEX_RADIUS;
		bound.width = bound.height = 4 * VERTEX_RADIUS;

		index = ind;

		name.setFont(vertexNameFont); //настройка названия вершины
		name.setString(std::to_string(index));
		name.setCharacterSize(VERTEX_NAME_SIZE);
		name.setOrigin(name.getLocalBounds().left + name.getLocalBounds().width/2.0f, name.getLocalBounds().top + name.getLocalBounds().height / 2.0f);
		name.setPosition(circle->getPosition());


		selected = false;
	}

	void select() {
		selected = true;
		circle->setFillColor(vertexColorSelected);
	}

	void deselect() {
		selected = false;
		circle->setFillColor(vertexColor);
	}

	/*~vertex() {
		delete circle;
	}*/
};

std::vector<vertex> vertices;
std::vector<vertex*> selectedVertices;

void drawGraph() {
	int index = 0;
	if (vertexNameFontLoaded){
		for (auto v : vertices) {
			app.draw(*v.circle);
			app.draw(v.name);
		}
	}
	else
		for (auto v : vertices) app.draw(*v.circle);
}

std::vector<vertex>::iterator findCollision(sf::Event::MouseButtonEvent press) {
	vector<vertex>::iterator v = vertices.begin();
	while (v != vertices.end()) {
		if ((*v).bound.contains(sf::Vector2f(press.x, press.y))) return v;
		v++;
	}
	return vertices.end();
}

void drawGrid() {

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};

	//app.draw(line, 2, sf::Lines);
}
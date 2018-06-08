#pragma once
//#include <SFML\Graphics.hpp>
#include "Settings.h"
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>

typedef sf::Vertex* edgeEnd; //конец ребра - указатель на координаты вершины

struct vertex {
	sf::Vertex coord; //координаты для соединения с ребрами
	sf::CircleShape* circle;
	sf::FloatRect bound;
	bool selected;

	int index;
	sf::Text name;

	vertex(int x, int y, int ind) {
		coord.position.x = x; //координаты для... 
		coord.position.y = y; //...соединения с ребрами
		coord.color = sf::Color::Black;

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

	void move(float x, float y) {
		circle->setPosition(x, y);

		bound = circle->getLocalBounds(); //настройка границы
		bound.left = circle->getPosition().x -  2 * VERTEX_RADIUS;
		bound.top = circle->getPosition().y - 2 * VERTEX_RADIUS;
		bound.width = bound.height = 4 * VERTEX_RADIUS;

		name.setPosition(x, y);

		coord.position.x = x;
		coord.position.y = y;
	}

	/*~vertex() {
		delete circle;
	}*/
};

std::vector<vertex> vertices; //графическое представление вершин

struct edge {
	edgeEnd line[2];

	edge(int ind1, int ind2) {
		line[0] = &(vertices[ind1].coord); 
		line[1] = &(vertices[ind2].coord);
	}

	sf::Text weight;


};

std::vector<edge> edges; //графическое представление ребер

void drawGraph() {
	for (auto e : edges) {
		sf::Vertex line[2] = {*(e.line[0]), *(e.line[1])};
		app.draw(line, 2, sf::Lines);
	}
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

void connectSelectedVertices() {
	for (auto v : vertices) {
		if (v.selected) {
			for (auto w : vertices) {
				if (v.index != w.index && w.selected) { //v!=w так петель нет
					edge e(v.index, w.index);
					edges.push_back(e);
				}
			}
		}
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
#include "Drawing.h"

void addVertexToScreen(vertex* v) { //добавить вершину на экран
	vertices.push_back(*v);
}

void deleteEdgeFromScreen(edge* e) { //удалить ребро с экрана
	std::list<edge>::iterator it = edges.begin();
	while (it != edges.end()) {
		if (it->getFirstVertex() == e->getFirstVertex() && it->getSecondVertex() == e->getSecondVertex()) { //если вершины совпадают, то ребро найдено
			edges.erase(it);
			break;
		}
		it++;
	}
}

std::list<edge>::iterator deleteEdgeFromScreen(std::list<edge>::iterator it) { //удалить ребро с экрана(через итератор)
	return edges.erase(it);
}

void deleteVertexFromScreen(vertex* v) { //удалить вершину с экрана
	std::list<edge>::iterator it = edges.begin();
	while (it != edges.end()) { //удалить ребра, инцидентные вершине
		if (it->getFirstVertex() == v || it->getSecondVertex() == v) it = deleteEdgeFromScreen(it);
		else it++;
	}

	//непосредственно удаление вершины
	std::list<vertex>::iterator itV = vertices.begin();//O(n) time complexity. 
	while (itV != vertices.end()) {
		if (itV->getIndex() == v->getIndex()) {
			vertices.erase(itV);
			break;
		}
		itV++;
	}
}

void addEdgeToScreen(edge* e) { //добавить ребро на экран
	edges.push_back(*e);
}


void drawGraph() { //отрисовка вершин и ребер
	for (auto e : edges) e.draw();
	for (auto v : vertices) v.draw();
}

InterfaceObject* findCollision(sf::Vector2f mouseAppPos) { //найти попадание в вершину
	std::list<vertex>::iterator v = vertices.begin();
	while (v != vertices.end()) {
		if (v->contains(mouseAppPos)) {
			return &(*v);
		}
		v++;
	}

	std::list<edge>::iterator e = edges.begin();
	while (e != edges.end()) {
		if (e->contains(mouseAppPos)) {
			return &(*e);
		}
		e++;
	}

	return nullptr;
}
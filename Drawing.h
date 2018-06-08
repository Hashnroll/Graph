#pragma once
#include "Settings.h"
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>

//������� ����� ��� ������������ ������� ����������(������, �����, ������)
class InterfaceObject {
protected:
	bool selected; //������� �� ������?
	sf::FloatRect bound; //������� ������������ ������������� �������
	sf::Vertex coord; //���������� ������������ �������
public:
	virtual void draw() = 0; //����������� ������� ��� ��������� �������
	virtual bool contains(sf::Vector2f mouseAppPos) = 0; //����������� ������� ��� ����������� ��������� ������� ���� �� ��������
	virtual std::string getType() = 0; //����������� ������� ��� ��������� ���� ������� �������(������� ��� �����������, �� ����� ������ ������� ����������� �������

	sf::Vector2f getCoord() { //�������� ���������� �������
		return coord.position;
	}

	bool isSelected() {
		return selected;
	}

	virtual void select() = 0; //�������� ������ ����������

	virtual void deselect() = 0; //�������� ��������� ������� ����������
};

//����� �������, ����������� InterfaceObject
class vertex : public InterfaceObject {
private:
	sf::CircleShape circle; //����������� ������������� ������� � ���� �����
	
	int index; //������ �������
	sf::Text name; //������� �� �������

public:
	vertex(int x, int y, int ind) {
		coord.position.x = x; //���������� ���... 
		coord.position.y = y; //...���������� � �������
		coord.color = sf::Color::Black; //���� �����

		circle.setRadius(VERTEX_RADIUS); //��������� ������������ �����������
		circle.setOrigin(VERTEX_RADIUS, VERTEX_RADIUS);
		circle.setPosition(x, y);
		circle.setFillColor(vertexColor);

		bound = circle.getLocalBounds(); //��������� �������
		bound.left = circle.getPosition().x - 2*VERTEX_RADIUS;
		bound.top = circle.getPosition().y - 2*VERTEX_RADIUS;
		bound.width = bound.height = 4 * VERTEX_RADIUS;

		index = ind;

		name.setFont(font); //��������� �������� �������
		name.setString(std::to_string(index));
		name.setCharacterSize(VERTEX_NAME_SIZE);
		name.setOrigin(name.getLocalBounds().left + name.getLocalBounds().width/2.0f, name.getLocalBounds().top + name.getLocalBounds().height / 2.0f);
		name.setPosition(circle.getPosition());


		selected = false;
	}

	int getIndex() {
		return index;
	}

	void select() {
		selected = true;
		circle.setFillColor(vertexColorSelected);
	}

	void deselect() {
		selected = false;
		circle.setFillColor(vertexColor);
	}

	void move(float x, float y) { //����������� �������
		coord.position.x = x;
		coord.position.y = y;

		circle.setPosition(x, y);

		bound = circle.getLocalBounds(); //��������� �������
		bound.left = circle.getPosition().x -  2 * VERTEX_RADIUS;
		bound.top = circle.getPosition().y - 2 * VERTEX_RADIUS;
		bound.width = bound.height = 4 * VERTEX_RADIUS;

		name.setPosition(x, y);

	}

	void draw() {
		app.draw(circle);
		if (fontLoaded) app.draw(name);
	}

	bool contains(sf::Vector2f mouseAppPos) override {
		return bound.contains(mouseAppPos.x, mouseAppPos.y);
	}

	std::string getType() {
		return "Vertex";
	}
};

std::vector<vertex> vertices; //����������� ������������� ������

//����� �����, ����������� InterfaceObject
class edge : public InterfaceObject { 
private:
	vertex *v1, *v2; //��������� �� ������� ������� �����
	sf::Vector2f normal; //������� � ������ ���������� ������ �����(��� ���������� ���������� �� �����)

	sf::Vertex line[2];//����������� �����������
	sf::Sprite arrow;

	int weight; //��� �����
	sf::Text weightText;//����������� ����
public:
	edge(vertex *ptr1, vertex *ptr2, int w) {
		v1 = ptr1;
		v2 = ptr2;

		normal.x = v2->getCoord().y - v1->getCoord().y;
		normal.y = v1->getCoord().x - v2->getCoord().x;
		
		line[0].color = line[1].color = edgeColor;

		arrow = arrowTemplate;

		weight = w;

		selected = false;
	}

	std::string getType() {
		return "Edge";
	}

	//������� ������� ������� �����
	vertex* getFirstVertex() {
		return v1;
	}
	vertex* getSecondVertex() { 
		return v2;
	}
	
	bool contains(sf::Vector2f mouseAppPos) override {
		sf::Vector2f v1Pos = line[0].position = v1->getCoord();
		sf::Vector2f v2Pos = line[1].position = v2->getCoord();
		normal.x = v2Pos.y - v1Pos.y;
		normal.y = v1Pos.x - v2Pos.x;

		int C = -1 * (normal.x*v1->getCoord().x+normal.y*v1->getCoord().y); //�������� C � ��������� ������
		int distance = abs(normal.x*mouseAppPos.x+normal.y*mouseAppPos.y+C)/(sqrt(normal.x*normal.x+ normal.y*normal.y));

		return (distance < EDGE_BOUND_VALUE) ? true : false;
	}

	void draw() {
		sf::Vector2f v1Pos = line[0].position = v1->getCoord();
		sf::Vector2f v2Pos = line[1].position = v2->getCoord();

		normal.x = v2Pos.y - v1Pos.y;
		normal.y = v1Pos.x - v2Pos.x;

		sf::Vector2f l(v2Pos.x-v1Pos.x, v2Pos.y-v1Pos.y); //������ ����
		double alpha = atan((v2Pos.y - v1Pos.y) / (v2Pos.x - v1Pos.x)) * 180.0 / 3.141593; //���� ������� ����
		alpha = v2Pos.x < v1Pos.x ? 180 + alpha  : alpha;
		alpha = v2Pos.y < v1Pos.y ? 360 + alpha : alpha;

		arrow.setOrigin(14,10); //��������� ����������� ������� �� ����
		arrow.setPosition(v1Pos.x+(2.0/3.0)*l.x, v1Pos.y + (2.0 / 3.0)*l.y);
		arrow.setRotation(alpha);

		weightText.setFont(font); //��������� �������� �������
		weightText.setFillColor(sf::Color::Black);
		weightText.setString(std::to_string(weight));
		weightText.setCharacterSize(EDGE_TEXT_SIZE);
		weightText.setOrigin(weightText.getLocalBounds().left + weightText.getLocalBounds().width / 2.0f, weightText.getLocalBounds().top + weightText.getLocalBounds().height / 2.0f);
		weightText.setPosition(arrow.getPosition().x+20*normal.x/sqrt(normal.x*normal.x+normal.y*normal.y), arrow.getPosition().y + 20*normal.y / sqrt(normal.x*normal.x + normal.y*normal.y));

		app.draw(arrow); //��������� �������
		app.draw(line, 2, sf::Lines); //��������� ����
		app.draw(weightText);
	}

	void select() {
		selected = true;
		line[0].color = line[1].color = edgeColorSelected;
	}

	void deselect() {
		selected = false;
		line[0].color = line[1].color = edgeColor;
	}
};

std::vector<edge> edges; //����������� ������������� �����

void addVertexToScreen(vertex* v) { //�������� ������� �� �����
	vertices.push_back(*v);
}

void deleteEdgeFromScreen(edge* e) { //������� ����� � ������
	vector<edge>::iterator it = edges.begin();
	while (it != edges.end()) { //����� � �������� �����, ����������� ������ �������
		if (it->getFirstVertex() == e->getFirstVertex()|| it->getSecondVertex() == e->getSecondVertex()) {
			edges.erase(it);
			break;
		}
		it++;
	}
}

vector<edge>::iterator deleteEdgeFromScreen(vector<edge>::iterator it) { //������� ����� � ������(����� ��������)
	return edges.erase(it);
}

void deleteVertexFromScreen(vertex* v) { //������� ������� � ������
	vector<edge>::iterator it = edges.begin();
	while (it!=edges.end()){ //������� �����, ����������� �������
		if (it->getFirstVertex() == v || it->getSecondVertex() == v) {
			it = deleteEdgeFromScreen(it);
		}
		else it++;
	}

	vertices.erase(vertices.begin() + std::distance(vertices.data(), v)); //������� ������� �� ������� ������
}

void addEdgeToScreen(edge* e) { //�������� ����� �� �����
	edges.push_back(*e);
}



void drawGraph() {
	for (auto e : edges) {
		e.draw();
	}
	for (auto v : vertices) {
		v.draw();
	}
}

InterfaceObject* findCollision(sf::Vector2f mouseAppPos) {
	vector<vertex>::iterator v = vertices.begin();
	while (v != vertices.end()) {
		if (v->contains(mouseAppPos)) return v._Ptr;
		v++;
	}

	vector<edge>::iterator e = edges.begin();
	while (e != edges.end()) {
		if (e->contains(mouseAppPos)) return e._Ptr;
		e++;
	}

	return nullptr;
}

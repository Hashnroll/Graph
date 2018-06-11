#pragma once
#include "Settings.h"

class InterfaceObject;

class vertex; 

class edge;


std::list<vertex> vertices; //����������� ������������� ������
std::list<edge> edges; //����������� ������������� �����

											 //������� ����� ��� ������������ ������� ����������(������, �����, ������)
class InterfaceObject
{
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
		bound.left = circle.getPosition().x - 2 * VERTEX_RADIUS;
		bound.top = circle.getPosition().y - 2 * VERTEX_RADIUS;
		bound.width = bound.height = 4 * VERTEX_RADIUS;

		index = ind;

		name.setFont(font); //��������� �������� �������
		name.setString(std::to_string(index));
		name.setCharacterSize(VERTEX_NAME_SIZE);
		name.setOrigin(name.getLocalBounds().left + name.getLocalBounds().width / 2.0f, name.getLocalBounds().top + name.getLocalBounds().height / 2.0f);
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

	void move(float x, float y); //implemented later cause uses edge class(which is after vertex class)

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


//����� �����, ����������� InterfaceObject
class edge : public InterfaceObject {
private:
	vertex * v1, *v2; //��������� �� ������� ������� �����
	sf::Vector2f normal; //������� � ������ ���������� ������ �����(��� ���������� ���������� �� �����)

	sf::Vertex line[2];//����������� �����������
	sf::Sprite arrow;

	int weight; //��� �����
	sf::Text weightText;//����������� ����
public:
	edge(vertex* ptr1, vertex* ptr2, int w) {
		v1 = ptr1;
		v2 = ptr2;

		sf::Vector2f v1Pos = line[0].position = v1->getCoord();
		sf::Vector2f v2Pos = line[1].position = v2->getCoord();
		line[0].color = line[1].color = edgeColor;

		normal.x = v2Pos.y - v1Pos.y;
		normal.y = v1Pos.x - v2Pos.x;
		double d = sqrt(normal.x*normal.x + normal.y*normal.y);
		normal.x = normal.x / d;
		normal.y = normal.y / d;


		sf::Vector2f l(v2Pos.x - v1Pos.x, v2Pos.y - v1Pos.y); //������ ����
		double alpha = atan((v2Pos.y - v1Pos.y) / (v2Pos.x - v1Pos.x)) * 180.0 / 3.141593; //���� ������� ����
		alpha = v2Pos.x < v1Pos.x ? 180 + alpha : alpha;
		alpha = v2Pos.y < v1Pos.y ? 360 + alpha : alpha;
		arrow = arrowTemplate;
		arrow.setOrigin(14, 10);
		arrow.setPosition(v1Pos.x + (2.0 / 3.0)*l.x, v1Pos.y + (2.0 / 3.0)*l.y);//��������� ����������� ������� �� ����
		arrow.setRotation(alpha);

		weight = w;

		weightText.setFont(font); //��������� �������� �������
		weightText.setFillColor(sf::Color::Black);
		weightText.setString(std::to_string(weight));
		weightText.setCharacterSize(EDGE_TEXT_SIZE);
		weightText.setOrigin(weightText.getLocalBounds().left + weightText.getLocalBounds().width / 2.0f, weightText.getLocalBounds().top + weightText.getLocalBounds().height / 2.0f);
		weightText.setPosition(arrow.getPosition().x + 20 * normal.x, arrow.getPosition().y + 20 * normal.y);

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

	int getWeight() {
		return weight;
	}

	bool contains(sf::Vector2f mouseAppPos) override {
		sf::Vector2f v1Pos = v1->getCoord();
		sf::Vector2f v2Pos = v2->getCoord();
		double distBetweenVertices = sqrt((v1Pos.x - v2Pos.x)*(v1Pos.x - v2Pos.x) + (v1Pos.y - v2Pos.y)*(v1Pos.y - v2Pos.y));
		double distToSecondVertex = sqrt((mouseAppPos.x - v2Pos.x)*(mouseAppPos.x - v2Pos.x) + (mouseAppPos.y - v2Pos.y)*(mouseAppPos.y - v2Pos.y));
		if (distToSecondVertex > 0.5*distBetweenVertices) return false;

		int C = -1 * (normal.x*v1->getCoord().x + normal.y*v1->getCoord().y); //�������� C � ��������� ������
		int distance = abs(normal.x*mouseAppPos.x + normal.y*mouseAppPos.y + C);

		return (distance < EDGE_BOUND_VALUE) ? true : false;
	}

	void draw() {

		app.draw(arrow); //��������� �������
		app.draw(line, 2, sf::Lines); //��������� ����
		app.draw(weightText);
	}

	void recompute() {
		sf::Vector2f v1Pos = line[0].position = v1->getCoord();
		sf::Vector2f v2Pos = line[1].position = v2->getCoord();

		normal.x = v2Pos.y - v1Pos.y;
		normal.y = v1Pos.x - v2Pos.x;
		double d = sqrt(normal.x*normal.x + normal.y*normal.y);
		normal.x = normal.x / d;
		normal.y = normal.y / d;

		sf::Vector2f l(v2Pos.x - v1Pos.x, v2Pos.y - v1Pos.y); //������ ����
		double alpha = atan((v2Pos.y - v1Pos.y) / (v2Pos.x - v1Pos.x)) * 180.0 / 3.141593; //���� ������� ����
		alpha = v2Pos.x < v1Pos.x ? 180 + alpha : alpha;
		alpha = v2Pos.y < v1Pos.y ? 360 + alpha : alpha;

		arrow.setPosition(v1Pos.x + (2.0 / 3.0)*l.x, v1Pos.y + (2.0 / 3.0)*l.y);//��������� ����������� ������� �� ����
		arrow.setRotation(alpha);

		weightText.setPosition(arrow.getPosition().x + 20 * normal.x, arrow.getPosition().y + 20 * normal.y);
	}

	void select() {
		selected = true;
		line[0].color = line[1].color = edgeColorSelected;
	}

	void deselect() {
		selected = false;
		line[0].color = line[1].color = edgeColor;
	}

	void inputWeight(tgui::TextBox::Ptr input) {
		tgui::Layout2d widgetPos = weightText.getPosition();
		widgetPos.x += 20;
		input->setPosition(widgetPos);
		if (!input->isVisible()) input->show();
		input->focus();

		if (input->getText().getSize() > 0) {
			weight = tgui::stoi(input->getText());
			weightText.setString(std::to_string(weight));
		}
	}

	bool operator==(edge e) {
		return (this->getFirstVertex() == e.getFirstVertex() && this->getSecondVertex() == e.getSecondVertex());
	}
};


void addVertexToScreen(vertex* v) { //�������� ������� �� �����
	vertices.push_back(*v);
}

void deleteEdgeFromScreen(edge* e) { //������� ����� � ������
	std::list<edge>::iterator it = edges.begin();
	while (it != edges.end()) {
		if (it->getFirstVertex() == e->getFirstVertex() && it->getSecondVertex() == e->getSecondVertex()) { //���� ������� ���������, �� ����� �������
			edges.erase(it);
			break;
		}
		it++;
	}
}

std::list<edge>::iterator deleteEdgeFromScreen(std::list<edge>::iterator it) { //������� ����� � ������(����� ��������)
	return edges.erase(it);
}

void deleteVertexFromScreen(vertex* v) { //������� ������� � ������
	std::list<edge>::iterator it = edges.begin();
	while (it != edges.end()) { //������� �����, ����������� �������
		if (it->getFirstVertex() == v || it->getSecondVertex() == v) it = deleteEdgeFromScreen(it);
		else it++;
	}

	//��������������� �������� �������
	std::list<vertex>::iterator itV = vertices.begin();//O(n) time complexity. 
	while (itV != vertices.end()) {
		if (itV->getIndex() == v->getIndex()) {
			vertices.erase(itV);
			break;
		}
		itV++;
	}
}

void addEdgeToScreen(edge* e) { //�������� ����� �� �����
	edges.push_back(*e);
}


void drawGraph() { //��������� ������ � �����
	for (auto e : edges) e.draw();
	for (auto v : vertices) v.draw();
}

InterfaceObject* findCollision(sf::Vector2f mouseAppPos) { //����� ��������� � �������
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


void vertex::move(float x, float y) {
	//����������� �������
	coord.position.x = x;
	coord.position.y = y;

	circle.setPosition(x, y);

	bound = circle.getLocalBounds(); //��������� �������
	bound.left = circle.getPosition().x - 2 * VERTEX_RADIUS;
	bound.top = circle.getPosition().y - 2 * VERTEX_RADIUS;
	bound.width = bound.height = 4 * VERTEX_RADIUS;

	name.setPosition(x, y);

	for (std::list<edge>::iterator it = edges.begin(); it != edges.end(); it++) {
		if (it->getFirstVertex() == this || it->getSecondVertex() == this) it->recompute();
	}
}
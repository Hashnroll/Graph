#pragma once
#include "Drawing.h"
#include "Settings.h"
#include "Graph.h"

void randomizeGraph(Graph* g) {
	g->clear();//������� �������� ���� 
	vertices.clear(); edges.clear();//������� ����������� �������������

	srand(time(NULL));//seed ��� rand()

										//�������� ���-�� ������
	int V = tgui::stoi(verticesRandomTextBox->getText()), E = rand() % (V*(V + 1) / 2);
	//�������� �������� ����� �����
	int weightLow = tgui::stoi(edgesLowTextBox->getText()), weightHigh = tgui::stoi(edgesHighTextBox->getText());

	int maxX = gui.getSize().x - 50, minX = 50, maxY = gui.getSize().y-50, minY = 70;

	//������ ����
	for (int i = 0; i < V; i++) {
		vertex v(rand() % (maxX - minX + 1) + minX, rand() % (maxY - minY + 1) + minY, g->getMaxIndex());//�������� �� �����
		addVertexToScreen(&v);

		g->addVertex();
	}

	for (int i = 0; i < E; i++) {
		int x, y;
		do {
			x = rand() % V; y = rand() % V;
		} while (g->connected(x, y));
		int weight = rand() % (weightHigh - weightLow + 1) + weightLow;
		g->addEdge(x, y, weight);

		std::list<vertex>::iterator v = vertices.begin(), w = vertices.begin();
		for (int k = 0; k < x; k++) v++;
		for (int k = 0; k < y; k++) w++;
		edge e(&(*v), &(*w), weight);

		addEdgeToScreen(&e);
	}
}
#pragma once
#include "Graph.h"
#include <queue>
#include <stack>
#include <map>

class FordBellman { //�� �������� � ������������� ������
private:
	map<int,int> distTo; //���������� �� ���� ������ �� ��������� s
	map<int,edgeGraph> edgeTo; //��������� ����� �� ���� �� s � �������
	map<int,bool> onQueue; //��������� �� ������� � ������� � ������ ������?
	std::queue<int> queue; //������� ������, ������� ����� �������������
	int cost; //���-�� ������� relax

public:
	FordBellman(Graph* g, int s) {
		for (int v : g->getVertices()) {
			distTo.insert(make_pair(v,INT_MAX));
			edgeTo.insert(make_pair(v, edgeGraph()));
			onQueue.insert(make_pair(v, false));
		}
		distTo.at(s) = 0;

		queue.push(s);
		onQueue.at(s) = true;

		while (!queue.empty()) {
			int v = queue.front();
			queue.pop();
			onQueue.at(v) = false;
			relax(g,v);
		}
	}

	void relax(Graph* g, int v) {
		for (edgeGraph e : g->getAdj(v)) {
			int w = e.vertexSecond();
			if (v!=w & distTo.at(w) > distTo.at(v) + e.getWeight()) {
				distTo.at(w) = distTo.at(v) + e.getWeight();
				edgeTo.at(w) = e;
				if (!onQueue.at(w)) {
					queue.push(w);
					onQueue.at(w) = true;
				}
			}
		}
	}

	int distToVertex(int v) {
		return distTo.at(v);
	}

	bool hasPathToVertex(int v) {
		return distTo.at(v) < INT_MAX;
	}

	stack<edgeGraph> pathToVertex(int v) {
		stack<edgeGraph> path;
		for (edgeGraph e = edgeTo.at(v); !e.isNull(); e = edgeTo.at(e.vertexFirst())) {
			path.push(e);
		}
		return path;
	}
};

void computeFordBellman(Graph* g);

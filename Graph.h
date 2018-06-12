#include <deque>
#include <map>
#include <iterator>
#include <algorithm>
#include <iostream>

using namespace std;

class edgeGraph {
private:
	int v; //������ �������, �� ������� ���� ����
	int w; //������ �������, � ������� ���� ����
	int weight; //��� ����
public:
	edgeGraph() {
		weight = INT_MAX;
	}

	bool isNull() {
		return (weight == INT_MAX);
	}

	edgeGraph(int indexFirst, int indexSecond, int weightValue) {
		v = indexFirst;
		w = indexSecond;
		weight = weightValue;
	}

	int vertexFirst() {
		return v;
	}

	int vertexSecond() {
		return w;
	}

	int getWeight() {
		return weight;
	}

	void setWeight(int w) {
		weight = w;
	}

	int compareTo(edgeGraph e) {
		if (this->weight < e.weight) return -1;
		else if (this->weight > e.weight) return 1;
		else return 0;
	}

	const bool operator==(edgeGraph e) {
		return ((v == e.vertexFirst()) && (w == e.vertexSecond()) && (weight = e.getWeight()));
	}

	void operator=(edgeGraph e) {
		v = e.vertexFirst(); w = e.vertexSecond(); weight = e.getWeight();
	}
};

class Graph {
private:

	map<int, deque<edgeGraph>> adj; //������ ������, ��� deque - ��������������� �������, � ������� �������� ��� �����, ����������� � �������� �� ������� i
	map<int, int> indegree; //���-�� ������, �������� � ��� �������. 1�� int-������ �������, 2�� int-���-�� ������, �������� � ���
	int V; //���-�� ������ � �����
	int E; //���-�� ����� � �����
	int maxIndex; //������������ ������

public:

	Graph() {
		V = 0;
		E = 0;
		maxIndex = 0;
	}

	deque<int> getVertices() {
		deque<int> vertices;
		for (auto it = adj.begin(); it != adj.end(); it++) vertices.push_back(it->first);
		return vertices;
	}

	deque<edgeGraph> getAdj(int v) {
		return adj.at(v);
	}

	void addVertex() { //�������� �������
		adj.insert(make_pair(maxIndex, deque<edgeGraph>())); //������� ������ ���� � ������
		indegree.insert(pair<int, int>(maxIndex, 0));//���������� � ������� V �� ������ ������� �������
		V++; //����������� ������� ������
		maxIndex++;
	}

	void deleteVertex(int i) {
		for (auto it = adj.begin(); it != adj.end(); it++)  //������ �� �������(�� ��������)
			if (connected(i, it->first) || connected(it->first,i)) {  //���� ������� ��������� � ���������, ����� ����� ���� ����� ������
				deleteEdge(i, it->first);
				deleteEdge(it->first, i);
			}

		for (auto it = adj.begin(); it != adj.end(); it++) { //������ �� �������� � �������� ����� �������
			if (it->first == i) {
				adj.erase(it);
				break;
			}
		}

		indegree.erase(i); //������� ������� i �� ��������� indegree
		V--;
	}

	void addEdge(int v, int w, int weight) { //���������� ����� �� v � w
		edgeGraph newEdge(v, w, weight);
		adj.at(v).push_back(newEdge);
		adj.at(w).push_back(newEdge);
		indegree.at(w)++; //������ � ������� w ������ �� ���� ������� ������
		E++;
	}

	void deleteEdge(int v, int w) { //�������� ����� �� v � w
		deque<edgeGraph>::iterator it_deque;
		for (it_deque = adj.at(v).begin(); it_deque != adj.at(v).end(); it_deque++)
			if (it_deque->vertexSecond() == w) {
				adj.at(v).erase(it_deque);
				break;
			}
		for (it_deque = adj.at(w).begin(); it_deque != adj.at(w).end(); it_deque++)
			if (it_deque->vertexFirst() == v) {
				adj.at(w).erase(it_deque);
				break;
			}
		indegree.at(w)--;
	}

	/*void vertexIdentification(int v, int w) { //�������������� ������
		deque<edgeGraph>::iterator it_w, it_v;
		for (it_w = adj.at(w).begin(); it_w != adj.at(w).end(); it_w++) //������ �� ���� ��������, ������� � w
			if (find(adj.at(v).begin(), adj.at(v).end(), *it_w) == adj.at(v).end()) //���� ���� ������� ��� � ������� � v
				adj.at(w).push_back(*it_w); //�������� �� ����
		deleteVertex(w); //������� w �� �����
	}*/

	int getVerticesAmount() { 
		return V;
	}

	int getMaxIndex() {
		return maxIndex;
	}

	bool connected(int ind1, int ind2) { //���������� �� ����� �� ind1 � ind2
		for (auto v : adj)
		{
			if (v.first == ind1) { //���� ������ �������
				for (auto e : v.second)
					if (e.vertexSecond() == ind2) return true;  //���� ������� ������ �������, ���� ������ � ������ ������� � ��� ������
				return false;
			}
		}
		return false;
	}

	void setWeight(int x, int y, int w) { //���������� ��� ��� ���� �� x � y
		for (std::deque<edgeGraph>::iterator it = adj.at(x).begin(); it != adj.at(x).end(); it++) {
			if (it->vertexSecond() == y) {
				it->setWeight(w);
				break;
			}
		}
		for (std::deque<edgeGraph>::iterator it = adj.at(y).begin(); it != adj.at(y).end(); it++) {
			if (it->vertexFirst() == x) {
				it->setWeight(w);
				break;
			}
		}
	}

	void print() { //����� �����
		for (map<int, deque<edgeGraph>>::iterator itAdj = adj.begin(); itAdj != adj.end(); itAdj++) {
			cout << itAdj->first << " : ";
			for (auto it = (itAdj->second).begin(); it != (itAdj->second).end(); it++) {
				cout << it->vertexFirst() << "->" << it->vertexSecond() << " " << it->getWeight() << ", ";
			}
			cout << endl;
		}
	}

};
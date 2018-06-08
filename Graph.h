#include <deque>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

class Graph {
private:
	class edgeGraph {
	private:
		int v; //������ �������, �� ������� ���� ����
		int w; //������ �������, � ������� ���� ����
		int weight; //��� ����
	public:
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

		int compareTo(edgeGraph e) {
			if (this->weight < e.weight) return -1;
			else if (this->weight > e.weight) return 1;
			else return 0;
		}

		const bool operator==(edgeGraph e) {
			return ((v==e.vertexFirst()) && (w==e.vertexSecond()) && (weight=e.getWeight()));
		}
	};

	vector<pair<int, deque<edgeGraph>>> adj; //������ ������, ��� deque - ��������������� �������, � ������� �������� ��� �����, ����������� � �������� �� ������� i
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


	void addVertex() { //�������� �������
		adj.push_back(make_pair(maxIndex, deque<edgeGraph>())); //������� ������ ���� � ������
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
		adj[v].second.push_back(newEdge);
		adj[w].second.push_back(newEdge);
		indegree.at(w)++; //������ � ������� w ������ �� ���� ������� ������
		E++;
	}

	void deleteEdge(int v, int w) { //�������� ����� �� v � w
		deque<edgeGraph>::iterator it_deque;
		for (it_deque = adj[v].second.begin(); it_deque != adj[v].second.end(); it_deque++)
			if ((*it_deque).vertexSecond() == w) {
				adj[v].second.erase(it_deque);
				break;
			}
		indegree.at(w)--;
		for (it_deque = adj[w].second.begin(); it_deque != adj[w].second.end(); it_deque++)
			if ((*it_deque).vertexFirst() == v) {
				adj[w].second.erase(it_deque);
				break;
			}
	}

	void vertexIdentification(int v, int w) { //�������������� ������
		deque<edgeGraph>::iterator it_w, it_v;
		for (it_w = adj[w].second.begin(); it_w != adj[w].second.end(); it_w++) //������ �� ���� ��������, ������� � w
			if (find(adj[v].second.begin(), adj[v].second.end(), *it_w) == adj[v].second.end()) //���� ���� ������� ��� � ������� � v
				adj[w].second.push_back(*it_w); //�������� �� ����
		deleteVertex(w); //������� w �� �����
	}

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

	void print() {
		for (int i = 0; i<V; i++) {
			cout<<adj[i].first << " : ";
			for (auto it = adj[i].second.begin(); it != adj[i].second.end(); it++) {
				cout<<(*it).vertexFirst() << "->"<<(*it).vertexSecond()<<" "<<(*it).getWeight()<<", ";
			}
			cout << endl;
		}

	}

};
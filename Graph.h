#include <deque>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

class Graph {
private:
	vector<pair<int, deque<int>>> adj; //������ ������, ��� deque - ��������������� �������, � ������� �������� ��� �������, ������� � �������� �� ������� i
	map<int, int> indegree; //���-�� ������, �������� � ��� �������. 1�� int-������ �������, 2�� int-���-�� ������, �������� � ���
	int V; //���-�� ������ � �����
	int E; //���-�� ����� � �����

public:

	Graph() {
		V = 0;
		E = 0;
		//adj.push_back(make_pair(0,deque<int>())); //0 ������� - �� ������������
	}


	void addVertex() { //�������� �������
		adj.push_back(make_pair(V, deque<int>())); //������� ������ ���� � ������
		indegree.insert(pair<int, int>(V, 0));//���������� � ������� V �� ������ ������� �������
		V++; //����������� ������� ������
	}

	void deleteVertex(int i) {
		if (i>0 && i<V) adj.erase(adj.begin() + i); //�������� �� ������� �������� � �������� i(�.�. ����, ������ ������� ������� - ������ i, � ������ - ���, � ������� �������� ��� ������� ������� ��� ������� i

		for (auto it = adj.begin(); it != adj.end(); it++) { //������ �� �������(�� ��������)
			deque<int>::iterator it_deque; //�������� ��� ����
			for (it_deque = it->second.begin(); it_deque != it->second.end(); it_deque++); { //������ �� ���� - �� ���� ������� �������� ��� ������
				if (*it_deque == i) {
					it->second.erase(it_deque);
					break;
				}
			}
		}

		indegree.erase(i); //������� ������� i �� ��������� indegree
	}

	void addEdge(int v, int w) { //���������� �����
		adj[v].second.push_back(w);
		indegree.at(w)++; //������ � ������� w ������ �� ���� ������� ������
		E++;
	}

	void deleteEdge(int v, int w) { //�������� ����� �� v � w
		deque<int>::iterator it_deque;
		for (it_deque = adj[v].second.begin(); it_deque != adj[v].second.end(); it_deque++)
			if (*it_deque == w) adj[v].second.erase(it_deque);
		indegree.at(w)--;
	}

	void vertexIdentification(int v, int w) { //�������������� ������
		deque<int>::iterator it_w, it_v;
		for (it_w = adj[w].second.begin(); it_w != adj[w].second.end(); it_w++) //������ �� ���� ��������, ������� � w
			if (find(adj[v].second.begin(), adj[v].second.end(), *it_w) == adj[v].second.end()) //���� ���� ������� ��� � ������� � v
				adj[w].second.push_back(*it_w); //�������� �� ����
		deleteVertex(w); //������� w �� �����
	}

	void print() {
		for (int i = 0; i<V; i++) {
			cout<<i << " : ";
			for (auto it = adj[i].second.begin(); it != adj[i].second.end(); it++) {
				cout<<*it << " ";
			}
			cout << endl;
		}

	}

};
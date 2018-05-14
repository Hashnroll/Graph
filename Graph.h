#include <deque>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

class Graph {
private:
	vector<pair<int, deque<int>>> adj; //список связей, где deque - двунаправленная очередь, в которой хранятся все вершины, смежные с вершиной по индексу i
	map<int, int> indegree; //кол-во вершин, входящих в эту вершину. 1ый int-индекс вершины, 2ой int-кол-во вершин, входящих в нее
	int V; //кол-во вершин в графе
	int E; //кол-во ребер в графе
	int maxIndex; //максимальный индекс из всех вершин

public:

	Graph() {
		V = 0;
		E = 0;
		maxIndex = 0;
	}


	void addVertex() { //добавить вершину
		adj.push_back(make_pair(maxIndex, deque<int>())); //вставка нового дека в вектор
		indegree.insert(pair<int, int>(maxIndex, 0));//изначально в вершину V не входят никакие вершины
		V++; //увеличиваем счетчик вершин
		maxIndex++;
	}

	void deleteVertex(int i) {
		if (i>0 && i<V) adj.erase(adj.begin() + i); //удаление из вектора элемента с индексом i(т.е. пары, первый элемент которой - индекс i, а второй - дек, в котором хранятся все смежные вершины для вершины i

		for (auto it = adj.begin(); it != adj.end(); it++) { //проход по вектору(по вершинам)
			deque<int>::iterator it_deque = it->second.begin(); //итератор для дека
			while (it_deque != it->second.end()) { //проход по деку - по всем смежным вершинам для данной
				if (*it_deque == i) {
					it->second.erase(it_deque);
					break;
				}
				it_deque++;
			}
		}

		indegree.erase(i); //удалить вершину i из структуры indegree
		V--;
	}

	void addEdge(int v, int w) { //добавление ребра
		adj[v].second.push_back(w);
		indegree.at(w)++; //теперь в вершину w входит на одну вершину больше
		E++;
	}

	void deleteEdge(int v, int w) { //удаление ребра из v в w
		deque<int>::iterator it_deque;
		for (it_deque = adj[v].second.begin(); it_deque != adj[v].second.end(); it_deque++)
			if (*it_deque == w) adj[v].second.erase(it_deque);
		indegree.at(w)--;
	}

	void vertexIdentification(int v, int w) { //отождествление вершин
		deque<int>::iterator it_w, it_v;
		for (it_w = adj[w].second.begin(); it_w != adj[w].second.end(); it_w++) //проход по всем вершинам, смежным с w
			if (find(adj[v].second.begin(), adj[v].second.end(), *it_w) == adj[v].second.end()) //если этой вершины нет в смежных с v
				adj[w].second.push_back(*it_w); //добавить ее туда
		deleteVertex(w); //удалить w из графа
	}

	int getVerticesAmount() {
		return V;
	}

	int getMaxIndex() {
		return maxIndex;
	}

	void print() {
		for (int i = 0; i<V; i++) {
			cout<<adj[i].first << " : ";
			for (auto it = adj[i].second.begin(); it != adj[i].second.end(); it++) {
				cout<<*it << " ";
			}
			cout << endl;
		}

	}

};
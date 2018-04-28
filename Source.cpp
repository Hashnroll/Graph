#include "Graph.h"

int main() {
	Graph g;
	
	for (int i=0; i<5;i++)
		g.addVertex();	
	
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.addEdge(4, 1);
	g.addEdge(2, 4);

	g.print();

	system("pause");

	return 0;
}
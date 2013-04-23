#include "Graph.h"
#include "Vehicle.h"
#include "graphviewer.h"

using namespace std;

Graph<int> CreateTestGraph() {
	Graph<int> myGraph;

	for (int i = 1; i < 8; i++) {
		myGraph.addVertex(i);
	}

	myGraph.addEdge(1, 2, 2);
	myGraph.addEdge(1, 4, 7);
	myGraph.addEdge(2, 4, 3);
	myGraph.addEdge(2, 5, 5);
	myGraph.addEdge(3, 1, 2);
	myGraph.addEdge(3, 6, 5);
	myGraph.addEdge(4, 3, 1);
	myGraph.addEdge(4, 5, 1);
	myGraph.addEdge(4, 6, 6);
	myGraph.addEdge(4, 7, 4);
	myGraph.addEdge(5, 7, 2);
	myGraph.addEdge(6, 4, 3);
	myGraph.addEdge(7, 6, 4);

	return myGraph;
}

GraphViewer* prepareGraphViewer(Graph<int>* graph) {
	GraphViewer *gv = new GraphViewer(600, 600, true);
	gv->createWindow(600, 600);
	gv->importGraph(*graph);
	gv->rearrange();
	return gv;
}

void shortestPath(GraphViewer* gv, Graph<int>* graph) {
	int source, destination, current;
	cout << "Primeiro nó:";
	cin >> source;
	cout << "Segundo nó:";
	cin >> destination;
	cin.ignore(INT_MAX, '\n');
	if(graph->getVertex(source)==NULL || graph->getVertex(destination)==NULL)
	{
		cout << "Nodes não existem" << endl << "Press enter to continue...";
		cin.get();
		return;
	}
	gv->setVertexColor(source, "green");
	gv->setVertexColor(destination, "red");
	gv->rearrange();
	graph->dijkstraShortestPath(source);

	//first iteration
	current = destination;
	cout << current;
	Vertex<int>* path;
	vector<int> visited_edges;
	Edge<int>* pathedge;
	bool found = true;
	int sum = 0;
	do {
		path = graph->getVertex(current)->path;
		pathedge = graph->getVertex(current)->pathedge;
		if (path == NULL) {
			cout << " Caminho não encontrado";
			found=false;
			break;
		}
		current = path->getInfo();
		cout << "<" << current;
		gv->setEdgeColor(pathedge->getId(), "red");
		sum+=pathedge->getWeight();
		visited_edges.push_back(pathedge->getId());
	} while (current != source);
	if(found)
		cout << endl << "Distância a percorrer: " << sum << endl;
	cout << endl << "Press enter to continue...";
	cin.get();
	gv->setVertexColor(source, DEFAULT_VERTEX_COLOR);
	gv->setVertexColor(destination, DEFAULT_VERTEX_COLOR);
	for (int i = 0; i < visited_edges.size(); i++)
		gv->setEdgeColor(visited_edges[i], DEFAULT_EDGE_COLOR);
	gv->rearrange();
}

int main() {
	Graph<int> test = CreateTestGraph();
	GraphViewer *gv = prepareGraphViewer(&test);
	int opt;
	while (true) {
		cout
				<< "Sistema de evacuação\n1-Abrir nova janela\n2-Caminho mais curto\n0-Sair\nOpção:";
		cin >> opt;
		switch (opt) {
		case 1:
			gv = prepareGraphViewer(&test);
			break;
		case 2: {
			shortestPath(gv, &test);
			break;
		}
		case 0:
			return 0;
		}
	}
}

void printSquareArray(int ** arr, unsigned int size) {
	for (unsigned int k = 0; k < size; k++) {
		if (k == 0) {
			cout << "   ";
			for (unsigned int i = 0; i < size; i++)
				cout << " " << i + 1 << " ";
			cout << endl;
		}

		for (unsigned int i = 0; i < size; i++) {
			if (i == 0)
				cout << " " << k + 1 << " ";

			if (arr[k][i] == INT_INFINITY)
				cout << " - ";
			else
				cout << " " << arr[k][i] << " ";
		}

		cout << endl;
	}
}


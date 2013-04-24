#include "Graph.h"
#include "Vehicle.h"
#include "graphviewer.h"
#include <algorithm>

#define VEHICLE_CAPACITY 5

using namespace std;

Graph<int> CreateTestGraph() {
	Graph<int> myGraph = *new Graph<int>();

	for (int i = 1; i < 8; i++) {
		myGraph.addVertex(i);
	}

	myGraph.getVertex(2)->addVehicle(Vehicle(VEHICLE_CAPACITY));
	myGraph.getVertex(4)->addVehicle(Vehicle(VEHICLE_CAPACITY));
	myGraph.getVertex(7)->addVehicle(Vehicle(VEHICLE_CAPACITY));

	myGraph.getVertex(3)->addPeople(35);

	myGraph.getVertex(5)->isShelter = true;

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
	if (graph->getVertex(source) == NULL
			|| graph->getVertex(destination) == NULL) {
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
			found = false;
			break;
		}
		current = path->getInfo();
		cout << "<" << current;
		gv->setEdgeColor(pathedge->getId(), "red");
		sum += pathedge->getWeight();
		visited_edges.push_back(pathedge->getId());
	} while (current != source);
	if (found)
		cout << endl << "Distância a percorrer: " << sum << endl;
	cout << endl << "Press enter to continue...";
	cin.get();
	gv->setVertexColor(source, DEFAULT_VERTEX_COLOR);
	gv->setVertexColor(destination, DEFAULT_VERTEX_COLOR);
	for (int i = 0; i < visited_edges.size(); i++)
		gv->setEdgeColor(visited_edges[i], DEFAULT_EDGE_COLOR);
	gv->rearrange();
}

int getClosestVehicle(Graph<int>* graph, vector<Vertex<int> *> v,
		Vertex<int>* p) {
	int source, destination, current, minsum = INT_MAX, minvertex = 0;

	destination = p->getInfo();
	for (int i = 0; i < v.size(); i++) {
		current = destination;
		source = v[i]->getInfo();
		Vertex<int>* path;
		vector<int> visited_edges;
		Edge<int>* pathedge;
		bool found = true;
		int sum = 0;
		graph->dijkstraShortestPath(source);
		do { //para as pessoas

			path = graph->getVertex(current)->path;
			pathedge = graph->getVertex(current)->pathedge;
			if (path == NULL) {
				found = false;
				break;
			}
			current = path->getInfo();
			sum += pathedge->getWeight();
			visited_edges.push_back(pathedge->getId());

		} while (current != source);
		if (found) {
			//cout << endl << "Distância a percorrer: " << sum << endl;
			if (sum < minsum) {
				minsum = sum;
				minvertex = source;
			}
		} else
			continue;
	}
	return minvertex;
}

void savePeople(GraphViewer* gv, Graph<int>* graph) {
	cin.ignore(INT_MAX, '\n');
	bool removedAll = false;
	int source, destination, current;
	Vertex<int> *p = NULL, *v = NULL, *s = NULL;
	vector<Vertex<int> *> vs;
	for (int i = 0; i < graph->getNumVertex(); i++) {
		Vertex<int>* current = graph->getVertexSet()[i];
		if (current->getPeople() > 0)
			p = current;
		else if (current->getVehicle()->getCapacity() > 0)
			vs.push_back(current);
		else if (current->isShelter)
			s = current;
	}
	if (p == NULL || vs.size() == 0 || s == NULL) {
		cout << "Nodes não existem" << endl << "Press enter to continue...";
		cin.get();
		return;
	}

	int closestVehicle, currentCapacity;
	Vertex<int>* path;
	vector<int> visited_edges;
	Edge<int>* pathedge;
	bool found = true;
	int sum;
	closestVehicle = getClosestVehicle(graph, vs, p);
	if (closestVehicle != 0) {
		v = graph->getVertex(closestVehicle);
		source = closestVehicle;
	} else {
		cout << " Veículo sem acesso" << endl;
		cout << endl << "Press enter to continue...";
		cin.get();
	}
	bool newVehicle = false; //new vehicle on shelter
	while (!removedAll) {
		destination = p->getInfo();
		sum = 0;
		graph->dijkstraShortestPath(source);

		//first iteration
		current = destination;
		cout << current;
		do { //para as pessoas
			path = graph->getVertex(current)->path;
			pathedge = graph->getVertex(current)->pathedge;
			if (path == NULL) {
				cout << " Caminho não encontrado";
				found = false;
				break;
			}
			current = path->getInfo();
			cout << "<" << current;
			gv->setEdgeColor(pathedge->getId(), "red");
			sum += pathedge->getWeight();
			visited_edges.push_back(pathedge->getId());
		} while (current != source);
		if (found) {
			gv->rearrange();
			cout << endl << "Distância a percorrer: " << sum << endl;
			cout << "Free seats: " << v->getVehicle()->getFreeSeats() << endl;
			cout << "Pessoas no abrigo" << p->getPeople() << endl;
			int removedP = v->getVehicle()->removeFreeSeats(p->getPeople());
			removedAll = removedP == p->getPeople();
			if (removedAll) {
				gv->setVertexColor(p->getInfo(), DEFAULT_VERTEX_COLOR);
				gv->rearrange();
			}
			p->removePeople(removedP);
			cout << "Removidas " << removedP << " pessoas." << endl;
			cout << "Pessoas no abrigo" << p->getPeople() << endl;
			cout << "Free seats: " << v->getVehicle()->getFreeSeats();
		}
		cout << endl << "Press enter to continue...";
		cin.get();
		for (int i = 0; i < visited_edges.size(); i++)
			gv->setEdgeColor(visited_edges[i], DEFAULT_EDGE_COLOR);
		visited_edges.clear();
		if (found) {
			source = destination;
			destination = s->getInfo();
			graph->dijkstraShortestPath(source);

			//first iteration
			current = destination;
			cout << current;
			sum = 0;
			do { //para o abrigo
				path = graph->getVertex(current)->path;
				pathedge = graph->getVertex(current)->pathedge;
				if (path == NULL) {
					cout << " Caminho não encontrado";
					found = false;
					break;
				}
				current = path->getInfo();
				cout << "<" << current;
				gv->setEdgeColor(pathedge->getId(), "green");
				sum += pathedge->getWeight();
				visited_edges.push_back(pathedge->getId());
			} while (current != source);
			if (found) {
				gv->rearrange();
				cout << endl << "Distância a percorrer: " << sum << endl;
				v->getVehicle()->resetFreeSeats();
				cout << "Pessoas entregues no abrigo" << endl;
			}
			cout << endl << "Press enter to continue...";
			cin.get();
			for (int i = 0; i < visited_edges.size(); i++)
				gv->setEdgeColor(visited_edges[i], DEFAULT_EDGE_COLOR);
			gv->rearrange();
			visited_edges.clear();
		}
		if (!removedAll) {
			source = s->getInfo();
			destination = p->getInfo();
			if (newVehicle) { //new vehicle departed on previous cycle
				v->getVehicle()->addCapacity(currentCapacity); //os veículos vão em conjunto / v é o ultimo veiculo a ter chegado
				newVehicle = false;
			}
			vs.push_back(s); //nó falso (representa o veículo actualmente no abrigo)
			vs.erase(find(vs.begin(), vs.end(), v));
			closestVehicle = getClosestVehicle(graph, vs, p);
			if (closestVehicle != 0) {
				if (closestVehicle != s->getInfo()) { //trocou de veículo
					currentCapacity = v->getVehicle()->getCapacity(); //capacidade da soma dos veículos no abrigo
					v = graph->getVertex(closestVehicle);
					source = closestVehicle;
					newVehicle = true;
				}
			}
			vs.erase(find(vs.begin(), vs.end(), s));
		}
		//TODO: Ver se o segundo veículo mais próximo é mais rápido do que múltiplas viagens
	}

}

int main() {
	Graph<int> test = CreateTestGraph();
	GraphViewer *gv = prepareGraphViewer(&test);
	int opt;
	while (true) {
		cout
				<< "Sistema de evacuação\n1-Abrir nova janela\n2-Salvar pessoas\n0-Sair\nOpção:";
		cin >> opt;
		switch (opt) {
		case 1:
			gv = prepareGraphViewer(&test);
			break;
		case 2: {
			savePeople(gv, &test);
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


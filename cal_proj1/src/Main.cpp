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

bool shortestPath(GraphViewer* gv, Graph<int>* graph, Vertex<int> **source,
		Vertex<int> **dest, vector<int> *visited_edges, string edge_color,
		int *dist = new int(), bool verbose = true) {
	bool found = true;
	*dist = 0;
	Vertex<int> *current, *path;
	Edge<int>* pathedge;
	graph->dijkstraShortestPath((*source)->getInfo());

	//first iteration
	current = *dest;
	if (verbose)
		cout << current->getInfo();
	do {
		path = current->path;
		pathedge = current->pathedge;
		if (path == NULL) {
			if (verbose)
				cout << endl << "Caminho não encontrado" << endl;
			found = false;
			break;
		}
		current = path;
		if (verbose)
			cout << "<" << current->getInfo();
		if (gv != NULL)
			gv->setEdgeColor(pathedge->getId(), edge_color);
		*dist += pathedge->getWeight();
		if (visited_edges != NULL)
			visited_edges->push_back(pathedge->getId());
	} while (current != *source);
	if (found) {
		if (gv != NULL)
			gv->rearrange();
		if (verbose)
			cout << endl << "Distância a percorrer: " << *dist << endl;
	}
	return found;
}

//for use in getClosestVehicle
bool shortestPath(Graph<int>* graph, Vertex<int> **source, Vertex<int> **dest,
		int *dist) {
	return shortestPath(NULL, graph, &(*source), &(*dest), NULL,
			DEFAULT_EDGE_COLOR, &(*dist), false);
}

int getClosestVehicle(Graph<int>* graph, vector<Vertex<int> *> v,
		Vertex<int>* p, Vertex<int>* s = NULL) {
	int sum, minsum = INT_MAX;
	Vertex<int> *current, *minvertex = NULL, *path;
	Edge<int>* pathedge;

	for (int i = 0; i < v.size(); i++) { //for each vertex (with vehicles), get shortest path to people
		sum = 0;
		if (shortestPath(graph, &v[i], &p, &sum)) {
			//cout << endl << "Distância a percorrer: " << sum << endl;
			if (sum < minsum && v[i] != s) {
				minsum = sum;
				minvertex = v[i];
			}
		}
	}
	if (minvertex != NULL)
		return minvertex->getInfo();
	else
		return 0;
}

bool populateNodes(Graph<int>* graph, Vertex<int> **p, Vertex<int> **s,
		vector<Vertex<int> *>* vehicles) {
	Vertex<int>* current;
	for (int i = 0; i < graph->getNumVertex(); i++) {
		current = graph->getVertexSet()[i];
		if (current->getPeople() > 0)
			*p = current;
		else if (current->getVehicle()->getCapacity() > 0)
			vehicles->push_back(current);
		else if (current->isShelter)
			*s = current;
	}

	return (*p != NULL && vehicles->size() > 0 && *s != NULL);
}

void promptContinue() {
	cout << "Press enter to continue..." << endl;
	cin.get();
}

void clearEdges(GraphViewer* gv, vector<int> *edges) {
	for (int i = 0; i < edges->size(); i++)
		gv->setEdgeColor((*edges)[i], DEFAULT_EDGE_COLOR);
	gv->rearrange();
	edges->clear();
}

void setVertexVectorColor(GraphViewer* gv, vector<Vertex<int> *> *nodes, string color)
{
	for(int i=0;i<nodes->size();i++)
		gv->setVertexColor((*nodes)[i]->getInfo(),color);
	gv->rearrange();
}

void savePeople(GraphViewer* gv, Graph<int>* graph) {
	cin.ignore(INT_MAX, '\n');
	bool removedAll = false, found = true;
	int closestVehicle, people, removedP;
	vector<int> visited_edges;
	Edge<int>* pathedge;
	Vertex<int> *p = NULL, *v = NULL, *s = NULL, *current, *path;
	vector<Vertex<int> *> vehicles;
	if (!populateNodes(graph, &p, &s, &vehicles)) { //graph not complete
		cout << "Grafo não completo" << endl;
		return promptContinue();
	}

	closestVehicle = getClosestVehicle(graph, vehicles, p);
	if (closestVehicle != 0) {
		v = graph->getVertex(closestVehicle);
	} else {
		cout << "Nenhum Veículo com acesso" << endl;
		return promptContinue();
	}
	while (!removedAll) { //repeat until all people saved
		found = shortestPath(gv, graph, &v, &p, &visited_edges, TO_PEOPLE_EDGE_COLOR); //To people
		if (found) {
			removedP = v->getVehicle()->removeFreeSeats(p->getPeople()); //How many people we can remove
			p->removePeople(removedP);
			people = p->getPeople();
			cout << "Evacuadas " << removedP << " pessoas." << endl;
			cout << "Ficaram " << people << " pessoas." << endl;
			removedAll = (people == 0);
			promptContinue();
			if (v != s)
				gv->setVertexColor(v->getInfo(), DEFAULT_VERTEX_COLOR);
			clearEdges(gv, &visited_edges);

			found = shortestPath(gv, graph, &p, &s, &visited_edges, TO_SHELTER_EDGE_COLOR); //To shelter
			if (found) {
				v->getVehicle()->resetFreeSeats();
				cout << "Pessoas entregues no abrigo." << endl;
				if (!removedAll) {
					if (v != s) {
						s->getVehicle()->addCapacity(
								v->getVehicle()->getCapacity()); //join vehicle capacity to other vehicles in the shelter
						vehicles.erase(
								find(vehicles.begin(), vehicles.end(), v));
						closestVehicle = getClosestVehicle(graph, vehicles, p,
								s);
						if (closestVehicle != 0)
							v = graph->getVertex(closestVehicle);
						else //only on first time that there are no vehicles closer from the shelter
						{
							v = s; //new vehicle base is shelter
							vehicles.push_back(v); //add vehicles in shelter to vector
						}
					}
				} else {
					gv->setVertexColor(p->getInfo(), DEFAULT_VERTEX_COLOR); //remove color from people vertex
					gv->rearrange();
				}
			} else
				break;
			promptContinue();
			clearEdges(gv, &visited_edges);
		} else
			break;
	}
	if (!found)
		promptContinue();
	s->addVehicle(Vehicle(0)); //clear vehicles from shelter;
	vehicles.clear();
	populateNodes(graph,&p,&s,&vehicles); //repopulate nodes
	setVertexVectorColor(gv,&vehicles,VEHICLE_COLOR); //reset vehicle colors
	cout << "Todas as pessoas salvas." << endl;
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

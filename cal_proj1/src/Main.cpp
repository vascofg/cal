#include "Graph.h"
#include "Vehicle.h"
#include "graphviewer.h"
#include <algorithm>




/**
 * Capacidade dos veículos definida pelo utilizador
 */
unsigned int vehicle_capacity; 
using namespace std;

Graph<int> CreateTestGraph() {
	Graph<int> myGraph = *new Graph<int>();

	for (int i = 1; i < 8; i++) {
		myGraph.addVertex(i);
	}

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

void addDefaultVehicles(Graph<int> *graph) {
	graph->getVertex(2)->addVehicle(Vehicle(vehicle_capacity));
	graph->getVertex(4)->addVehicle(Vehicle(vehicle_capacity));
	graph->getVertex(7)->addVehicle(Vehicle(vehicle_capacity));
}

GraphViewer* prepareGraphViewer(Graph<int>* graph) {
	GraphViewer *gv = new GraphViewer(600, 600, true);
	gv->createWindow(600, 600);
	gv->importGraph(*graph);
	gv->rearrange();
	return gv;
}
	/**
	 * Encontra o trajecto mais curto entre o nó de origem e o de destino
	 */	
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


	/**
	 * Encontra o trajecto mais curto entre o nó de origem e o de destino, para uso em getClosestVehicle()
	 */	
bool shortestPath(Graph<int>* graph, Vertex<int> **source, Vertex<int> **dest,
		int *dist) {
	return shortestPath(NULL, graph, &(*source), &(*dest), NULL,
			DEFAULT_EDGE_COLOR, &(*dist), false);
}


	/**
	 * Encontra o veículo mais próximo das pessoas.
	 */	
Vertex<int>* getClosestVehicle(Graph<int>* graph, vector<Vertex<int> *> v,
		Vertex<int>* p, Vertex<int>* s = NULL) {
	int sum, minsum = INT_MAX;
	Vertex<int> *minvertex = NULL;

	for (unsigned int i = 0; i < v.size(); i++) { //for each vertex (with vehicles), get shortest path to people
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
		return minvertex;
	else
		return NULL;
}

	/**
	 * Faz uma lista do nó mais próximo ao mais afastado.
	 */	
vector<Vertex<int> *> getClosestNodes(Graph<int>* graph,
		vector<Vertex<int> *> nodes, Vertex<int> *dest) {
	int dist;
	for (unsigned int i = 0; i < nodes.size(); i++) {
		if (shortestPath(graph, &nodes[i], &dest, &dist)) {
			nodes[i]->setDistAux(dist);
		} else
			nodes.erase(nodes.begin() + i); //delete node (not suitable)
	}
	sort(nodes.begin(), nodes.end(), vertex_dist_smaller_than<int>());
	return nodes;
}

	/**
	 * Acrescenta o número dado pelo utilizador de veículos ao grafo.
	 */	
bool addVehicles(Graph<int>* graph) {
	vector<Vertex<int> *> vehicles;
	Vertex<int> *current, *p;
	unsigned int nvehicles;
	cout << "Número de pontos estratégicos: ";
	cin >> nvehicles;
	for (int i = 0; i < graph->getNumVertex(); i++) { //populate nodes
		current = graph->getVertexSet()[i];
		if (current->getPeople() > 0)
			p = current;
		else if (!current->isShelter)
			vehicles.push_back(current);
	}
	vehicles = getClosestNodes(graph, vehicles, p);
	if (nvehicles > vehicles.size() || nvehicles < 1) {
		cout << "Número de veículos inválido" << endl;
		return addVehicles(graph);
	}
	for (unsigned int i = 0; i < nvehicles; i++) {
		graph->getVertex(vehicles[i]->getInfo())->addVehicle(
				Vehicle(vehicle_capacity));
	}
	return true;
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
	for (unsigned int i = 0; i < edges->size(); i++)
		gv->setEdgeColor((*edges)[i], DEFAULT_EDGE_COLOR);
	gv->rearrange();
	edges->clear();
}

void setVertexVectorColor(GraphViewer* gv, vector<Vertex<int> *> *nodes,
		string color) {
	for (unsigned int i = 0; i < nodes->size(); i++)
		gv->setVertexColor((*nodes)[i]->getInfo(), color);
	gv->rearrange();
}

	/**
	 * Imprime detalhes das pessoas e dos veículos.
	 */	
void printStats(Graph<int>* graph) {
	Vertex<int> *p = NULL, *s = NULL;
	vector<Vertex<int> *> vehicles;
	if (!populateNodes(graph, &p, &s, &vehicles))
		cout << "Grafo não completo!" << endl;
	else {
		if (p->getPeople() == 1)
			cout << "Existe 1 pessoa para socorrer";
		else
			cout << "Existem " << p->getPeople() << " pessoas para socorrer";
		cout << ", no nó " << p->getInfo() << endl;
		if (vehicles.size() == 1)
			cout << "O veículo está localizado no nó ";
		else
			cout << "Os veículos estão localizados nos nós ";
		for (unsigned int i = 0; i < vehicles.size(); i++)
			cout << vehicles[i]->getInfo() << ", ";
		cout << "com capacidade para " << vehicle_capacity << " pessoas"
				<< endl;
	}
}

	/**
	 * Função principal encarregue de decidir qual veículo deve ajudar as pessoas até estas estarem por completo no abrigo.
	 */	
void savePeople(GraphViewer* gv, Graph<int>* graph) {
	cin.ignore(INT_MAX, '\n');
	bool removedAll = false, found = true;
	int people, removedP;
	vector<int> visited_edges;
	Vertex<int> *p = NULL, *v = NULL, *s = NULL, *closestVehicle;
	vector<Vertex<int> *> vehicles;
	if (!populateNodes(graph, &p, &s, &vehicles)) { //graph not complete
		cout << "Grafo não completo" << endl;
		return promptContinue();
	}

	closestVehicle = getClosestVehicle(graph, vehicles, p);
	if (closestVehicle != NULL) {
		v = closestVehicle;
	} else {
		cout << "Nenhum Veículo com acesso" << endl;
		return promptContinue();
	}
	while (!removedAll) { //repeat until all people saved
		found = shortestPath(gv, graph, &v, &p, &visited_edges,
				TO_PEOPLE_EDGE_COLOR); //To people
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

			found = shortestPath(gv, graph, &p, &s, &visited_edges,
					TO_SHELTER_EDGE_COLOR); //To shelter
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
						if (closestVehicle != NULL)
							v = closestVehicle;
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
	populateNodes(graph, &p, &s, &vehicles); //repopulate nodes
	setVertexVectorColor(gv, &vehicles, VEHICLE_COLOR); //reset vehicle colors
	cout << "Todas as pessoas salvas." << endl;
}	
void startUp(Graph<int> *graph) {
	int opt;
	bool exit = false;
	while (!exit) { //Vehicle capacity option
		cout << "Capacidade dos veículos: ";
		cin >> opt;
		cin.ignore(INT_MAX, '\n');
		if (opt > 0) {
			vehicle_capacity = opt;
			exit = true;
		}
	}
	exit = false;
	while (!exit) { //Strategic points option
		cout
				<< "1-Pontos estratégicos estáticos\n2-Pontos estratégicos dinâmicos\nOpção: ";
		cin >> opt;
		cin.ignore(INT_MAX, '\n');
		switch (opt) {
		case 1:
			addDefaultVehicles(graph);
			exit = true;
			break;
		case 2:
			addVehicles(graph);
			exit = true;
			break;
		default:
			cout << "Opção inválida!" << endl;
		}
	}
}

int main() {
	Graph<int> graph = CreateTestGraph();
	int opt;
	startUp(&graph);
	GraphViewer *gv = prepareGraphViewer(&graph);
	while (true) { //Main menu option
		cout
				<< "Sistema de evacuação\n1-Abrir nova janela\n2-Salvar pessoas\n3-Estatísticas do grafo\n0-Sair\nOpção:";
		cin >> opt;
		switch (opt) {
		case 1:
			gv = prepareGraphViewer(&graph);
			break;
		case 2: {

			savePeople(gv, &graph);
			break;
		}
		case 3: {
			printStats(&graph);
			break;
		}
		case 0:
			return 0;
		default:
			cout << "Opção inválida!" << endl;
			break;
		}
	}
}

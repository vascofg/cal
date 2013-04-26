#include "Graph.h"
#include "Vehicle.h"
#include "graphviewer.h"
#include <algorithm>
#include <fstream>

#define FILE_NAME "graph.txt"

using namespace std;

/**
 * Função criadora de grafos, para efeito de testes.
 * @param: Grafo
 */
void CreateTestGraph(Graph<int> *myGraph) {

	for (int i = 1; i < 8; i++) {
		myGraph->addVertex(i);
	}

	myGraph->getVertex(3)->addPeople(35);

	myGraph->getVertex(5)->isShelter = true;

	myGraph->addEdge(1, 2, 2);
	myGraph->addEdge(1, 4, 7);
	myGraph->addEdge(2, 4, 3);
	myGraph->addEdge(2, 5, 5);
	myGraph->addEdge(3, 1, 2);
	myGraph->addEdge(3, 6, 5);
	myGraph->addEdge(4, 3, 1);
	myGraph->addEdge(4, 5, 1);
	myGraph->addEdge(4, 6, 6);
	myGraph->addEdge(4, 7, 4);
	myGraph->addEdge(5, 7, 2);
	myGraph->addEdge(6, 4, 3);
	myGraph->addEdge(7, 6, 4);
}

/**
 * Adiciona veículos predefinidos ao grafo.
 * @param: Grafo.
 */
void addDefaultVehicles(Graph<int> *graph) {
	graph->getVertex(2)->addVehicle(Vehicle(graph->vehicle_capacity));
	graph->getVertex(4)->addVehicle(Vehicle(graph->vehicle_capacity));
	graph->getVertex(7)->addVehicle(Vehicle(graph->vehicle_capacity));
}

/**
 * Prepara a janela do GraphViewer.
 * @param: Grafo.
 * @return Retorna o graphviewer preparado para o grafo passado por parâmetro
 */
GraphViewer* prepareGraphViewer(Graph<int>* graph) {
	GraphViewer *gv = new GraphViewer(600, 600, true);
	gv->createWindow(600, 600);
	gv->importGraph(*graph);
	gv->rearrange();
	return gv;
}

/**
 * Encontra o trajecto mais curto entre o nó de origem e o de destino
 * @param: GraphViewer.
 * @param: Grafo.
 * @param: Vértice de origem.
 * @param: Vértice de destino.
 * @param: Arestas visitadas.
 * @param: Cor da aresta.
 * @return: Retorna true caso encontre o caminho, false caso contrário
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
 * @param: Grafo.
 * @param: Vértice de origem.
 * @param: Vértice de destino.
 * @param: Distância.
 * @return: Retorna true caso encontre o caminho, false caso contrário
 */
bool shortestPath(Graph<int>* graph, Vertex<int> **source, Vertex<int> **dest,
		int *dist) {
	return shortestPath(NULL, graph, &(*source), &(*dest), NULL,
			DEFAULT_EDGE_COLOR, &(*dist), false);
}

/**
 * Encontra o veículo mais próximo das pessoas.
 * @param: Grafo.
 * @param: Vértice do veículo.
 * @param: Vértice das pessoas.
 * @param: Vértice do abrigo.
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
 * @param: Grafo.
 * @param: Vértices.
 * @param: Vértice de destino.
 * @return: Retorna um vector com os nós por ordem de distância ao vértice de destino
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
 * @param Grafo.
 * @return: Retorna true se os veículos foram adicionados com sucesso
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
				Vehicle(graph->vehicle_capacity));
	}
	return true;
}

/**
 * Adiciona os nós pessoas, abrigo e veículos às respectivas variáveis.
 * @param Grafo.
 * @param Referência para apontador do vértice de pessoas.
 * @param Referência para apontador do vértice de abrigo.
 * @param Referência para vector de vértices de veículos.
 * @return Retorna true se o grafo estiver completo (nó de pessoas, abrigo e pelo menos um veículo)
 */
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

/**
 * Pede ao utilizador para carregar numa tecla para continuar
 */
void promptContinue() {
	cout << "Press enter to continue..." << endl;
	cin.get();
}

/**
 * Limpa as cores das arestas para a default.
 * @param: GraphViewer.
 * @param: Vector das arestas.
 */
void clearEdges(GraphViewer* gv, vector<int> *edges) {
	for (unsigned int i = 0; i < edges->size(); i++)
		gv->setEdgeColor((*edges)[i], DEFAULT_EDGE_COLOR);
	gv->rearrange();
	edges->clear();
}

/**
 * Define a cor do vértice.
 * @param: GraphViewer.
 * @param: Vector de vértices.
 */
void setVertexVectorColor(GraphViewer* gv, vector<Vertex<int> *> *nodes,
		string color) {
	for (unsigned int i = 0; i < nodes->size(); i++)
		gv->setVertexColor((*nodes)[i]->getInfo(), color);
	gv->rearrange();
}

/**
 * Imprime detalhes das pessoas e dos veículos.
 * @param: Grafo.
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
		cout << "com capacidade para " << graph->vehicle_capacity << " pessoas"
				<< endl;
	}
}

/**
 * Função principal encarregue de decidir qual veículo deve ajudar as pessoas até estas estarem por completo no abrigo.
 * @param: GraphViewer.
 * @param: Grafo.
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

/**
 * Inicializa a interação com o utilizador
 * @param: Grafo.
 */
void startUp(Graph<int> *graph) {
	int opt;
	bool exit = false;
	while (!exit) { //Vehicle capacity option
		cout << "Capacidade dos veículos: ";
		cin >> opt;
		cin.ignore(INT_MAX, '\n');
		if (opt > 0) {
			graph->vehicle_capacity = opt;
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

/**
 * Carrega o grafo de um ficheiro.
 * @param: Grafo.
 * @return: Retorna true se o grafo for carregado com sucesso, false em caso contrário
 */
bool loadGraph(Graph<int>* graph) {
	ifstream ifs;
	string line;
	ifs.open(FILE_NAME, ios::in);
	if (!ifs.is_open())
		return false;
	getline(ifs, line); //vehicle capacity
	if (line == "")
		return false;
	graph->vehicle_capacity = atoi(line.c_str());
	getline(ifs, line); //blank line
	if (line != "")
		return false;
	getline(ifs, line); //first vertex
	if (line == "")
		return false;
	while (line != "") {
		size_t current = 0;
		size_t next = -1;
		next = line.find_first_of(" ", current);
		int vid = atoi(line.substr(current, next).c_str());
		graph->addVertex(vid);
		Vertex<int> *v = graph->getVertex(vid);
		if (next != string::npos) {
			current = next + 1;
			next = line.find_first_of(" ", current);
			char ntype = line.substr(current, next)[0];
			if (ntype == 's')
				v->isShelter = true;
			else if (next != string::npos) {
				current = next + 1;
				next = line.find_first_of(" ", current);
				unsigned int val = atoi(line.substr(current, next).c_str());
				if (ntype == 'v')
					v->addVehicle(Vehicle(val));
				else if (ntype == 'p')
					v->addPeople(val);
			}
		}
		getline(ifs, line);
	}
	getline(ifs, line); //first edge
	int eid = 0;
	while (line != "") {
		size_t current = 0;
		size_t next = -1;
		next = line.find_first_of(" ", current);
		eid = atoi(line.substr(current, next).c_str());
		current = next + 1;
		next = line.find_first_of(" ", current);
		int source = atoi(line.substr(current, next).c_str());
		current = next + 1;
		next = line.find_first_of(" ", current);
		int dest = atoi(line.substr(current, next).c_str());
		current = next + 1;
		next = line.find_first_of(" ", current);
		int weight = atoi(line.substr(current, next).c_str());
		graph->addEdge(source, dest, weight, eid);
		getline(ifs, line);
	}
	if (eid != 0)
		graph->numEdge = eid; //set numedge to the last edge on file
	return true;
}

/**
 * Guarda o grafo num ficheiro.
 * @param: Grafo.
 * @return: Retorna true se o ficheiro for gravado com sucesso, false em caso contrário
 */
bool saveGraph(Graph<int> *graph) {
	vector<Vertex<int> *> nodes = graph->getVertexSet();
	ofstream ofs;
	ofs.open(FILE_NAME, ios::out | ios::trunc);
	if (!ofs.is_open())
		return false;
	ofs << graph->vehicle_capacity << endl << endl;
	for (unsigned int i = 0; i < nodes.size(); i++) {
		ofs << nodes[i]->getInfo();
		if (nodes[i]->getPeople() > 0)
			ofs << " p " << nodes[i]->getPeople();
		else if (nodes[i]->getVehicle()->getCapacity() > 0)
			ofs << " v " << nodes[i]->getVehicle()->getCapacity();
		else if (nodes[i]->isShelter)
			ofs << " s";
		ofs << endl;
	}
	ofs << endl;
	for (unsigned int i = 0; i < nodes.size(); i++)
		for (unsigned int j = 0; j < nodes[i]->getAdj().size(); j++)
			ofs << nodes[i]->getAdj()[j].getId() << " " << nodes[i]->getInfo()
					<< " " << nodes[i]->getAdj()[j].getDest()->getInfo() << " "
					<< nodes[i]->getAdj()[j].getWeight() << endl;
	ofs.close();
	return true;
}

/**
 * Adiciona pessoas ao grafo. Se for seleccionado o nó atual, incrementa o num de pessoas no nó.
 * Se não remove as pessoas do nó atual e adiciona ao nó seleccionado
 * @param: GraphViewer.
 * @param: Grafo.
 */
void addPeople(GraphViewer *gv, Graph<int> *graph) {
	Vertex<int> *vertex, *current;
	int n;
	cout << "Nó: ";
	cin >> n;
	vertex = graph->getVertex(n);
	if (vertex != NULL && vertex->getVehicle()->getCapacity() == 0
			&& !vertex->isShelter) {
		cout << "Número de pessoas: ";
		cin >> n;
		if (n > 0) {
			if (vertex->getPeople() > 0) //if chosen vertex is the current one, increment to the value
				vertex->addPeople(n);
			else { //else, remove current people location and add people to the selected node
				for (int i = 0; i < graph->getNumVertex(); i++) { //populate nodes
					current = graph->getVertexSet()[i];
					if (current->getPeople() > 0) {
						current->removePeople(current->getPeople()); //remove current people location
						gv->setVertexColor(current->getInfo(),
								DEFAULT_VERTEX_COLOR);
						break;
					}
				}
			}

			vertex->addPeople(n);
			gv->setVertexColor(vertex->getInfo(), PEOPLE_COLOR);
			gv->rearrange();
		}
		else
			cout << "Número de pessoas inválido" << endl;
	}
	else
		cout << "Nó inválido!" << endl;
}

/**
 * Função main
 */
int main() {
	Graph<int> *graphpointer = NULL;
	Graph<int> graph = *new Graph<int>();
	string opt_load;
	bool loaded = false;
	if (loadGraph(&graph)) {
		cout << "Carregar do ficheiro(s/n): ";
		cin >> opt_load;
		if (opt_load == "s" || opt_load == "S") {
			graphpointer = &graph;
			cout << "Grafo carregado do ficheiro" << endl;
			loaded = true;
		} else
			graph = *new Graph<int>();
	}
	if (!loaded) {
		CreateTestGraph(&graph);
		graphpointer = &graph;
		startUp(graphpointer);
	}
	int opt;
	GraphViewer *gv = prepareGraphViewer(graphpointer);
	while (true) { //Main menu option
		cout
				<< "Sistema de evacuação\n1-Abrir nova janela\n2-Salvar pessoas\n3-Estatísticas do grafo\n4-Adicionar pessoas\n0-Sair\nOpção:";
		cin >> opt;
		switch (opt) {
		case 1:
			gv = prepareGraphViewer(graphpointer);
			break;
		case 2: {
			savePeople(gv, graphpointer);
			break;
		}
		case 3: {
			printStats(graphpointer);
			break;
		}
		case 4:
			addPeople(gv, graphpointer);
			break;
		case 0:
			saveGraph(graphpointer);
			return 0;
		default:
			cout << "Opção inválida!" << endl;
			break;
		}
	}
}

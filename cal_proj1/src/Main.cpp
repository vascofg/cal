#include "Graph.h"
#include "Vehicle.h"
#include "graphviewer.h"

using namespace std;

Graph<int> CreateTestGraph()
{
	Graph<int> myGraph;

	for(int i = 1; i < 8; i++)
	{
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

int main()
{
	Graph<int> test = CreateTestGraph();
	GraphViewer *gv = new GraphViewer(600, 600, true, test);
	return 0;
}

void printSquareArray(int ** arr, unsigned int size)
{
	for(unsigned int k = 0; k < size; k++)
	{
		if(k == 0)
		{
			cout <<  "   ";
			for(unsigned int i = 0; i < size; i++)
				cout <<  " " << i+1 << " ";
			cout << endl;
		}

		for(unsigned int i = 0; i < size; i++)
		{
			if(i == 0)
				cout <<  " " << k+1 << " ";

			if(arr[k][i] == INT_INFINITY)
				cout << " - ";
			else
				cout <<  " " << arr[k][i] << " ";
		}

		cout << endl;
	}
}


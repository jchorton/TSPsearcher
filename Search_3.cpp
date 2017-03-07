/******************************************************************************
C++ program for Determining TSP alomost optimal paths using Kruskal's
algorithm to find Minimum Spanning Tree of a given complete, undirected,
weighted graph.
Compile: g++ Search_3.cpp -o TSP_3 -g -Wall -std=c++0x
******************************************************************************/
#include <iostream>
using std::cout;
#include <fstream>
#include <vector>
using std::vector;
#include <cmath>

#define DEBUG_A 0
#define DEBUG_B 1

struct Edge {
	int cityTo;
	int cityFrom;
	int distance;
};

struct Node {
	int city;
	int X;
	int Y;
	vector<Edge> edgeList;
};

// Prototypes;
void readFile(std::string fileName, vector<Node> &graphNodes);
void edgeGen(vector<Node> &graphNodes);
int distCalc(struct Node A, struct Node B);

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Usage: TSP [inputFile.txt]\n";
		exit(1);
	}

	// Convert the filename to a string to keep track of it.
	std::string fileName = argv[1];

	vector<Node> graphNodes;

	readFile(fileName, graphNodes);

#if DEBUG_A
	for (unsigned int i = 0; i < graphNodes.size(); i++) {
		cout << graphNodes[i].city << ' ' << graphNodes[i].X << ' ' << graphNodes[i].Y << '\n';
		cout << "edge[0] " << graphNodes[i].edgelist[0].cityFrom << '\n';
	}
#endif

	edgeGen(graphNodes);

#if DEBUG_B
	for (unsigned int i = 0; i < graphNodes.size(); i++) {
		for (unsigned int j = 0; j < graphNodes.size(); j++) {
			cout << "cityFrom = " << graphNodes[i].edgeList[j].cityFrom << '\n';
			cout << "cityTo = " << graphNodes[i].edgeList[j].cityTo << '\n';
			cout << "distance = " << graphNodes[i].edgeList[j].distance << '\n';
		}
	}
#endif

	return 0;
}


void readFile(std::string fileName, vector<Node> &graphNodes) {
	std::ifstream inputFile;
	inputFile.open(fileName);
	if (!inputFile) {
		std::cerr << "ERROR: Cannot open file \"" << fileName << "\"" << '\n';
		exit(1);
	}

	int a, b, c;
	while (inputFile >> a >> b >> c) {
		Node *aNode = new Node;
		aNode->city = a;
		aNode->X = b;
		aNode->Y = c;
		graphNodes.push_back(*aNode);
	}

	// edgeList vector initialization in each Node.
	for (unsigned int i = 0; i < graphNodes.size(); i++) {
		for (unsigned int j = 0; j < graphNodes.size(); j++) {
			Edge *nullEdge = new Edge;
			nullEdge->cityFrom = -1;
			nullEdge->cityTo = -1;
			nullEdge->distance = -1;
			graphNodes[i].edgeList.push_back(*nullEdge);
		}
	}

	inputFile.close();
}

void edgeGen(vector<Node> &graphNodes) {
	for (unsigned int i = 0; i < graphNodes.size(); i++) {
		for (unsigned int j = 0; j < graphNodes.size(); j++) {
			graphNodes[i].edgeList[j].cityFrom = i;
			graphNodes[i].edgeList[j].cityTo = j;
			graphNodes[i].edgeList[j].distance = distCalc(graphNodes[i], graphNodes[j]);
		}
	}
}

int distCalc(struct Node cityA, struct Node cityB) {
	return round(sqrt(pow(cityA.X - cityB.X, 2) + pow(cityA.Y - cityB.Y, 2)));
}
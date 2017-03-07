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

#define DEBUG 0;

struct Node {
	int city;
	int X;
	int Y;
	vector<Edge> edgelist;
};

struct Edge {
	Node cityTo;
	Node cityFrom;
	int distance;
};

// Prototypes;
void readFile(std::string fileName, vector<Node> &graphNodes);

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Usage: TSP [inputFile.txt]\n";
		exit(1);
	}

	// Convert the filename to a string to keep track of it.
	std::string fileName = argv[1];

	vector<Node> graphNodes;

	readFile(fileName, graphNodes);

#if DEBUG
	for(unsigned int i = 0; i < graphNodes.size(); i++)
		cout << graphNodes[i].city << ' ' << graphNodes[i].X << ' ' << graphNodes[i].Y << '\n';
#endif

	//edgelist goes here!

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

	inputFile.close();
}
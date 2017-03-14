/******************************************************************************
* TSP.h is the header file for TSP.cpp
* CS-325-400-W17	Project 4		17 March 2017
* Jonathan Horton	hortonjo@oregonstate.edu
* Chris Kearns		kearnsc@oregonstate.edu
* Dustin Pack		packdu@oregonstate.edu
******************************************************************************/
#ifndef TSP_H
#define TSP_H

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
using std::ifstream;
using std::ios;
using std::ofstream;
#include <iomanip>
#include <iostream>
using std::cout;
#include <limits>
#include <pthread.h>
#include <queue>
#include <stack>
using std::stack;
#include <string>
using std::string;
#include <stdio.h>
#include <vector>
using std::vector;

#define NUM_THREADS 1
#define RUN_SHORTER 5

class TSP {
private:
	struct City {
		int X;
		int Y;
	};

	string inFileName;
	string outFileName;
	vector<int>oddNodes;
	void findOddNodes();
	int nearestNextNode(int *index, bool *MSTBuffer);

protected:

public:
	TSP(string inFile, string outFile);
	~TSP();
	// Variables and containers.
	int V;
	vector<City>cities;
	vector<int>circuit;
	vector<int> *adjacencyList;
	int **graph;
	int pathLength;
	int beginIndex[NUM_THREADS];
	int endIndex[NUM_THREADS];
	int **allPathLengths;
	// Methods.
	int calcDistance(struct City c1, struct City c2);
	void readCityData();
	void loadMatrix();
	void findMST();
	void matchMST();
	int findBestPath(int);
	void tourGen(int);
	void eulerCircuit(int pos, vector<int> &);
	void hamiltonCircuit(vector<int> &, int&);
	int pathIsShorter(int **graph, int v1, int v2, int v3, int v4, int &total_distance);
	int _2Opt(int **graph, vector<int> &path, int &pathLength, int n);
	void edgeSwap(vector<int> &path, int start, int end, int n);
	void optimize();
	void printResult();
};
#endif
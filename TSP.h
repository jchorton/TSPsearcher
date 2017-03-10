/******************************************************************************
* TSP.h is the header file for TSP.cpp - implements Christofides algorithm.
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

#define DEBUG 0
#define NUM_THREADS 5
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
	void getNodeCount();
	void findOddNodes();
	int nearestNextNode(int *index, bool *MSTBuffer);
protected:

public:
	TSP(string in, string out);
	~TSP();
	// Variables and containers:
	int V;
	vector<City>cities;
	int **graph;
	int pathLength;
	vector<int>circuit;
	vector<int> *adjacencyList;
	int startIndex[NUM_THREADS];
	int endIndex[NUM_THREADS];
	int **allPathLengths;
	// Methods:
	int calcDistance(struct City city1, struct City city2);
	void readCities();
	void loadMatrix();
	void findMST();
	void matchMST();
	int findBestPath(int);
	void tourGen(int);
	void eulerPath(int pos, vector<int> &);
	void hamiltonPath(vector<int> &, int&);
	int _2Opt(int **graph, vector<int> &path, int &pathLength, int V);
	void call2Opt();
	bool pathIsShorter(int **graph, int v1, int v2, int v3, int v4, int &pathLength);
	void edgeSwap(vector<int> &path, int start, int end, int V);
	void printResult();
};
#endif
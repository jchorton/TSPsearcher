/******************************************************************************
* TSP is a approximation solution for the special case Euclidian Distance 
* Traveleing Salesman Problem utilizing the Christofides algorithm [8]:
*   Create a minimum spanning tree T of G.
*   Let O be the set of vertices with odd degree in T.
*   Find min-weight matching M in induced subgraph given by vertices from O.
*   Combine edges of M and T to form a connected multigraph H.
*   Form an Eulerian circuit in H.
*   Make the circuit into a Hamiltonian circuit by skipping repeated vertices.
* main.cpp, dependent on Threads.h, Threads.cpp, TSP.h, and TSP.cpp
* CS-325-400-W17	Project 4		17 March 2017
* Jonathan Horton	hortonjo@oregonstate.edu
* Chris Kearns		kearnsc@oregonstate.edu
* Dustin Pack		packdu@oregonstate.edu
* Requires input file 3 x n, space delmited, int city, int x-coor, int y-coor.
******************************************************************************/
#include <climits>
#include "TSP.h"
#include "Threads.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Usage: ./TSP [inputfile]\n"
			<< "TSP requires Euclidian distance calculations, so\n"
			<< "inputfile format is space delimited INTEGERS like:\n"
			<< "city0 X-coordinate Y-coordinate\n"
			<< "city1 X-coordinate Y-coordinate\n"
			<< "with only 3 ints per line & at least 2 lines." << "\n";
		exit(1);
	}

	// Start timing
	clock_t start, stop;
	start = clock();

	// Read inut file names.
	string inFile, outFile;
	inFile = outFile = argv[1];
	outFile.append(".tour");

	// Create new tsp object.
	TSP tsp(inFile, outFile);
	int V = tsp.V;

	// Read cities from input file.
	tsp.readCities();
	cout << "Number of cities read from file: " << tsp.V << "\n";

	// Populate V x V matrix with calculated edge distances.
	tsp.loadMatrix();

	// Find an MST in graph using Prim's algorithm.
	tsp.findMST();

	// Find a "perfect" minimum weighted matching MST.
	tsp.matchMST();

	// Create array of thread objects.
	Thread threads[NUM_THREADS];

	int pathLength = INT_MAX;
	int bplIndex;
	int terminus = NUM_THREADS;

	/* This if/else block used to fine tune the execution time of TSP
		when running higher values of input cities vs. the desired
		level of accuracy.  Test against known optimal solutions.*/
	// Advance starting node incVar for each run by:
	int incVar;
	if (V < 500)
		incVar = 1;
	else if (V >= 500 && V < 1000)
		incVar = 2;
	else if (V >= 1000 && V < 1500)
		incVar = 4;
	else if (V >= 1500 && V < 2000)
		incVar = 8;
	else if (V >= 2000 && V < 2500)
		incVar = 16;
	else if (V >= 2500 && V < 3000)
		incVar = 32;
	else if (V >= 3000 && V < 3500)
		incVar = 64;
	else if (V >= 3500 && V < 4000)
		incVar = 128;
	else if (V >= 4000 && V < 4500)
		incVar = 256;
	else if (V >= 4500 && V < 5000)
		incVar = 512;
	else if (V >= 5000)
		incVar = 1024;

	// Start at node:
	int node = 0;

	// Count to get thread ids.
	int count = 0;

	// Thread manager - incrementaly advance by (NUM_THREADS * incVar) nodes.
	int spool = V;
	while (spool >= incVar) {
		if (spool < (NUM_THREADS * incVar)) {
			terminus = spool / incVar;
		}
		for (int i = 0; i < terminus; i++) {
			threads[i].aThread = &tsp;
			threads[i].startNode = node;
			threads[i].TSPid = count;
			threads[i].start(); // See Thread::start().
			node += incVar;
			count++;
		}
		// Wait for all threads to finish.
		for (int t = 0; t < terminus; t++) {
			threads[t].join();
		}
		spool -= (terminus * incVar);
	}

	cout << "Number of cities processed: " << count << "\n";

	// Find shortest path!
	for (int i = 0; i < count; i++) {
		if (tsp.allPathLengths[i][1] < pathLength) {
			bplIndex = tsp.allPathLengths[i][0];
			pathLength = tsp.allPathLengths[i][1];
		}
	}

	// Store best pathLength.
	tsp.tourGen(bplIndex);

	// Attempt to further optimze solution.
	for (int i = 0; i < RUN_SHORTER; i++) {
		tsp.call2Opt();
	}

	// stdout >> *.tour file.
	tsp.printResult();

	stop = clock();
	cout << "TSP's Best Path Length: " << pathLength << " when starting at city " << bplIndex << "\n";
	cout << "Time to solution: " << ((float)(stop - start)) / CLOCKS_PER_SEC << " seconds.\n";
	
	return 0;
}

/* CITATIONS: Code adapted from the following:
[1] Various lecture materials CS325-400-W17, J. Shulfort, Oregon State University.
[2] http://man7.org/linux/man-pages/man3/pthread_create.3.html
[3] http://man7.org/linux/man-pages/man3/strerror.3.html
[4] https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm
[5] https://www.go4expert.com/articles/writing-multithreaded-program-cpp-t29980/
[6] Lecture materials and programs covered in CS344-400-F16, B. Brewster, Oregon State University.
[7] "C++ Multithreading Cookbook", 2014, Milos Ljumovic, various.
[8] https://en.wikipedia.org/wiki/Christofides_algorithm
[9] http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/
[10] http://www.sanfoundry.com/cpp-program-find-mst-prims-algorithm/
[11] http://www.geeksforgeeks.org/eulerian-path-and-circuit/
[12] http://www.sanfoundry.com/cpp-program-find-hamiltonian-cycle/
[13] http://www.geeksforgeeks.org/backtracking-set-7-hamiltonian-cycle/
[14] http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems/
*/
/******************************************************************************
* TSP is an implementation of the approximation solution for the special case
* Euclidian Distance Traveling Salesman Problem utilizing the Christofides
* algorithm [10]:
*  Create a minimum spanning tree T of complete undirected graph G.
*  Let O be the set of vertices with odd degree in T. Set O will have an even
number of vertices.
*  Find a minimum-weight perfect matching M in the induced subgraph given by
the vertices from O.
*  Combine the edges of M and T to form a connected multigraph H in which each
vertex has even degree.
*  Form an Eulerian circuit in H.
*  Make the Eulerian circuit into a Hamiltonian circuit by skipping repeated
vertices (shortcutting).
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

	bool incVar_overRide = false;

	if (argc < 2 || argc > 4) {
		cout << "Usage: ./TSP [inputfile] [optional int 1]\n"
			<< "TSP requires Euclidian distance calculations, so\n"
			<< "inputfile format is space delimited INTEGERS like:\n"
			<< "city0 X-coordinate Y-coordinate\n"
			<< "city1 X-coordinate Y-coordinate\n"
			<< "with only 3 ints per line & at least 2 lines.\n";
		exit(1);
	}
	else if (argc == 3) {
		if (atoi(argv[2]) < 1 || atoi(argv[2]) > 1) {
			cout << "Optional argument must be 1 or omitted!\n";
			exit(1);
		}
		else {
			incVar_overRide = true;
		}
	}

	// Start timing
	clock_t start, stop;
	start = clock();

	// Read in file names.
	string inFile, outFile;
	inFile = outFile = argv[1];
	outFile.append(".tour");

	// Create tsp object.
	TSP tsp(inFile, outFile);
	int V = tsp.V;

	// Read cities from input file.
	tsp.readCityData();
	cout << "Number of cities read from file: " << tsp.V << "\n";

	// Fill N x N matrix with distances between nodes
	tsp.loadMatrix();

	// Find a MST T in graph G
	tsp.findMST();

	// Find a minimum weighted matching M for odd vertices in T
	tsp.matchMST();

	// Create array of thread objects
	Thread threads[NUM_THREADS];

	int best = INT_MAX;
	int bestIndex;
	int stop_here = NUM_THREADS;

	// Amount to increment starting node by each time
	int increment = 1; // by 1 if n < 1040

	if (V >= 600 && V < 1040)
		increment = 3;
	else if (V >= 1040 && V < 1800)
		increment = 8;
	else if (V >= 1800 && V < 3205)
		increment = 25; 		// ~ 220s @ 3200
	else if (V >= 3205 && V < 4005)
		increment = 50; 		// ~ 230s @ 4000
	else if (V >= 4005 && V < 5005)
		increment = 120;		// ~ 200 @ 5000
	else if (V >= 5005 && V < 6500)
		increment = 250;		// ~ 220s @ 6447
	else if (V >= 6500)
		increment = 500;

	int remaining = V;

	// Start at node zero
	int node = 0;

	// Count to get thread ids
	int count = 0;

	while (remaining >= increment) {
		// Keep track iteration when last node will be reached
		if (remaining < (NUM_THREADS * increment)) {

			// each iteration advances NUM_THREADS * increment nodes
			stop_here = remaining / increment;
		}

		for (long t = 0; t < stop_here; t++) {
			//cout << "Thread " << count << " starting at node " << node << endl;
			threads[t].startNode = node;
			threads[t].TSPid = count;
			threads[t].aThread = &tsp;
			threads[t].start();
			node += increment;
			count++;
		}

		// Wait for all the threads
		for (long t = 0; t < stop_here; t++) {
			threads[t].join();
		}
		remaining -= (stop_here * increment);
	}

	cout << "count: " << count << endl;
	// Loop through each index used and find shortest path
	for (long t = 0; t < count; t++) {
		if (tsp.allPathLengths[t][1] < best) {
			bestIndex = tsp.allPathLengths[t][0];
			best = tsp.allPathLengths[t][1];
		}
	}

	stop = clock();
	cout << "\nbest: " << best << " @ index " << bestIndex << "\n";
	cout << "time: " << ((float)(stop - start)) / CLOCKS_PER_SEC << "s\n";

	// Store best path
	tsp.tourGen(bestIndex);
	tsp.optimize();

	cout << "\nFinal length: " << tsp.pathLength << "\n";

	// Print to file
	tsp.printResult();

	return 0;
}

/* CITATIONS: Code adapted from the following:
[1] Various lecture materials, algorithms, and programs, CS325-400-W17, J. Shulfort, et. al. @Oregon State University.
[2] h ttp://man7.org/linux/man-pages/man3/pthread_create.3.html
[3] h ttp://man7.org/linux/man-pages/man3/pthread_join.3.html
[4] h ttp://man7.org/linux/man-pages/man3/strerror.3.html
[5] h ttps://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm
[6] h ttps://www.go4expert.com/articles/writing-multithreaded-program-cpp-t29980/
[7] Various lecture materials and programs, CS344-400-F16, B. Brewster, et. al. @Oregon State University.
[8] "C++ Multithreading Cookbook", 2014, M. Ljumovic.
[9] "C++ Concurrency in Action: Practical Multithreading", 1st Ed., A. Williams.
[10] h ttps://en.wikipedia.org/wiki/Christofides_algorithm
[11] h ttp://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/
[12] h ttp://www.sanfoundry.com/cpp-program-find-mst-prims-algorithm/
[13] h ttp://www.geeksforgeeks.org/eulerian-path-and-circuit/
[14] h ttp://www.sanfoundry.com/cpp-program-find-hamiltonian-cycle/
[15] h ttp://www.geeksforgeeks.org/travelling-salesman-problem-set-2-approximate-using-mst/
[16] h ttp://www.geeksforgeeks.org/branch-bound-set-5-traveling-salesman-problem/
[17] h ttp://www.geeksforgeeks.org/backtracking-set-7-hamiltonian-cycle/
[18] h ttp://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems/
[19] h ttp://www.jot.fm/issues/issue_2003_07/column8/

Optimal tour lengths:
tsp_example_1.txt = 108159
tsp_example_2.txt = 2579
tsp_example_3.txt = 1573084
*/
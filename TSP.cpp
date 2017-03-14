/******************************************************************************
* TSP.cpp is the implementation file for TSP.h
* CS-325-400-W17	Project 4		17 March 2017
* Jonathan Horton	hortonjo@oregonstate.edu
* Chris Kearns		kearnsc@oregonstate.edu
* Dustin Pack		packdu@oregonstate.edu
******************************************************************************/
#include "TSP.h"

struct thread_data {
	int tid;
	TSP *tsp;
};
struct thread_data *data;

TSP::TSP(string in, string out) {
	inFileName = in;
	outFileName = out;

	int count = 0;
	ifstream inStream;
	inStream.open(inFileName.c_str(), ios::in);

	if (!inStream) {
		cout << "Can't open input file " << inFileName << "\n";
		exit(1);
	}
	std::string unused;
	while (std::getline(inStream, unused))
		++count;
	V = count;
	inStream.close();

	graph = new int*[V];
	for (int i = 0; i < V; i++) {
		graph[i] = new int[V];
		for (int j = 0; j < V; j++)
			graph[i][j] = 0;
	}

	allPathLengths = new int*[V];
	for (int i = 0; i < V; i++) {
		allPathLengths[i] = new int[V];
	}

	adjacencyList = new vector<int>[V];
};

TSP::~TSP() {
	for (int i = 0; i < V; i++) {
		delete[] graph[i];
		delete[] allPathLengths[i];
	}
	delete[] allPathLengths;
	delete[] graph;
	delete[] adjacencyList;
}

void TSP::readCityData() {
	ifstream inStream;
	inStream.open(inFileName.c_str(), ios::in);
	if (!inStream) {
		cout << "Can't open input file " << inFileName << "\n";
		exit(1);
	}
	int c, x, y;
	int i = 0;
	while (!inStream.eof()) {
		inStream >> c >> x >> y;
		struct City c = { x, y };
		cities.push_back(c);
		i++;
	}
	inStream.close();
};

// Returns calculated distance given two pairs of city x|y coordinates.
int TSP::calcDistance(struct TSP::City cityA, struct TSP::City cityB) {
	return round(sqrt(pow(cityA.X - cityB.X, 2) + pow(cityA.Y - cityB.Y, 2)));
};

void *F(void* args) {
	struct thread_data *my_data = (struct thread_data *) args;
	int tid = my_data->tid;
	TSP *tsp = my_data->tsp;
	int **graph = tsp->graph;
	int start, end;

	start = tsp->beginIndex[tid];
	end = tsp->endIndex[tid];

	for (int i = start; i <= end; i++) {
		for (int j = i; j < tsp->V; j++) {
			graph[i][j] = graph[j][i] = tsp->calcDistance(tsp->cities[i], tsp->cities[j]);
		}
	}
	pthread_exit(NULL);
}

void TSP::loadMatrix() {
	int amount = (V / NUM_THREADS) * 0.2;
	int x = (V / NUM_THREADS) - amount;
	int rem = V - (x * NUM_THREADS);
	int half = NUM_THREADS / 2 + 1;

	int pos = 0;
	for (int i = 0; i < half; i++) {
		beginIndex[i] = pos;
		pos += (x - 1);
		endIndex[i] = pos;
		pos++;
	}
	int remainingThreads = NUM_THREADS - half + 1;
	int extraToEach = rem / remainingThreads;
	for (int i = half; i < NUM_THREADS; i++) {
		beginIndex[i] = pos;
		pos += (x + extraToEach - 1);
		endIndex[i] = pos;
		pos++;
	}
	endIndex[NUM_THREADS - 1] = V - 1;

	int rc; void *status;
	data = new struct thread_data[V];

	pthread_t *thread = new pthread_t[V];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (long t = 0; t < NUM_THREADS; t++) {
		data[t].tid = t;
		data[t].tsp = this;
		rc = pthread_create(&thread[t], &attr, F, (void*)&data[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_attr_destroy(&attr);
	for (long t = 0; t < NUM_THREADS; t++) {
		rc = pthread_join(thread[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	delete[] data;
}

void TSP::findMST() {
	int key[V];
	bool in_mst[V];
	int parent[V];

	for (int v = 0; v < V; v++) {
		key[v] = std::numeric_limits<int>::max();
		in_mst[v] = false;
	}

	key[0] = 0;
	parent[0] = -1;

	for (int i = 0; i < V - 1; i++) {
		int v = nearestNextNode(key, in_mst);
		in_mst[v] = true;
		for (int u = 0; u < V; u++) {
			if (graph[v][u] && in_mst[u] == false && graph[v][u] < key[u]) {
				parent[u] = v;
				key[u] = graph[v][u];
			}
		}
	}

	for (int v1 = 0; v1 < V; v1++) {
		int v2 = parent[v1];
		if (v2 != -1) {
			adjacencyList[v1].push_back(v2);
			adjacencyList[v2].push_back(v1);
		}
	}
};

int TSP::nearestNextNode(int key[], bool mstSet[]) {
	int min = std::numeric_limits<int>::max();
	int min_index;
	for (int v = 0; v < V; v++)
		if (mstSet[v] == false && key[v] < min) {
			min = key[v];
			min_index = v;
		}
	return min_index;
};

void TSP::findOddNodes() {
	for (int r = 0; r < V; r++) {
		if ((adjacencyList[r].size() % 2) != 0) {
			oddNodes.push_back(r);
		}
	}
}

void TSP::matchMST() {
	int closest, length; //int d;
	std::vector<int>::iterator tmp, first;

	findOddNodes();

	while (!oddNodes.empty()) {
		first = oddNodes.begin();
		vector<int>::iterator it = oddNodes.begin() + 1;
		vector<int>::iterator end = oddNodes.end();
		length = std::numeric_limits<int>::max();
		for (; it != end; ++it) {
			if (graph[*first][*it] < length) {
				length = graph[*first][*it];
				closest = *it;
				tmp = it;
			}
		}
		adjacencyList[*first].push_back(closest);
		adjacencyList[closest].push_back(*first);
		oddNodes.erase(tmp);
		oddNodes.erase(first);
	}
}


void TSP::eulerCircuit(int pos, vector<int> &path) {
	vector<int> *temp = new vector<int>[V];
	for (int i = 0; i < V; i++) {
		temp[i].resize(adjacencyList[i].size());
		temp[i] = adjacencyList[i];
	}

	path.clear();

	stack<int> stk;
	while (!stk.empty() || temp[pos].size() > 0) {
		if (temp[pos].size() == 0) {
			path.push_back(pos);
			int last = stk.top();
			stk.pop();
			pos = last;
		}
		else {
			stk.push(pos);
			int neighbor = temp[pos].back();
			temp[pos].pop_back();
			for (unsigned int i = 0; i < temp[neighbor].size(); i++)
				if (temp[neighbor][i] == pos) {
					temp[neighbor].erase(temp[neighbor].begin() + i);
					break;
				}
			pos = neighbor;
		}
	}
	path.push_back(pos);
}


void TSP::hamiltonCircuit(vector<int> &path, int &path_dist) {
	bool visited[V];
	memset(visited, 0, V * sizeof(bool));

	path_dist = 0;

	int root = path.front();
	vector<int>::iterator curr = path.begin();
	vector<int>::iterator next = path.begin() + 1;
	visited[root] = true;

	while (next != path.end()) {
		if (!visited[*next]) {
			path_dist += graph[*curr][*next];
			curr = next;
			visited[*curr] = true;
			next = curr + 1;
		}
		else {
			next = path.erase(next);
		}
	}
	path_dist += graph[*curr][*next];
}


void TSP::tourGen(int pos) {
	eulerCircuit(pos, circuit);
	hamiltonCircuit(circuit, pathLength);
}


int TSP::findBestPath(int pos) {
	vector<int>path;
	eulerCircuit(pos, path);
	int length;
	hamiltonCircuit(path, length);

	_2Opt(graph, path, length, V);
	_2Opt(graph, path, length, V);
	_2Opt(graph, path, length, V);
	_2Opt(graph, path, length, V);
	_2Opt(graph, path, length, V);

	return length;
}

void TSP::optimize() {
	for (int i = 0; i < RUN_SHORTER; i++) {
		_2Opt(graph, circuit, pathLength, V);
	}
}

void TSP::edgeSwap(vector<int> &path, int start, int end, int n) {
	while (end - start > 0) {
		int temp = path[start % n];
		path[start % n] = path[end % n];
		path[end % n] = temp;
		start++;
		end--;
	}
}


int TSP::pathIsShorter(int **graph, int v1, int v2, int v3, int v4, int &total_dist) {
	if ((graph[v1][v3] + graph[v2][v4]) < (graph[v1][v2] + graph[v3][v4])) {
		total_dist -= (graph[v1][v2] + graph[v3][v4] - graph[v1][v3] - graph[v2][v4]);
		return 1;
	}
	return 0;
}


int TSP::_2Opt(int **graph, vector<int> &path, int &pathLength, int V) {
	int counter = 0;
	int term_cond = 5;
	int old_distance = pathLength;
	int v1, v2, u1, u2;

	for (int i = 0; i < V; i++) {
		u1 = i;
		v1 = (i + 1) % V;

		for (int j = i + 2; (j + 1) % V != i; j++) {

			u2 = j % V;
			v2 = (j + 1) % V;

			if (pathIsShorter(graph, path[u1], path[v1], path[u2], path[v2], pathLength)) {

				edgeSwap(path, i + 1, j, V); // v1, u2

				if (pathLength - old_distance < 5 && counter == term_cond)
					break;

				if (pathLength - old_distance > term_cond)
					i = 0;

				old_distance = pathLength;
				counter++;
			}
		}
	}
	return pathLength;
}

void TSP::printResult() {
	ofstream outputStream;
	outputStream.open(outFileName.c_str(), ios::out);
	outputStream << pathLength << "\n";
	for (vector<int>::iterator it = circuit.begin(); it != circuit.end(); ++it) {
		outputStream << *it << "\n";
	}
	outputStream.close();
};
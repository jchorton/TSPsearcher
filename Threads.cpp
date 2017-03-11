/******************************************************************************
* Threads.cpp is the thread handler inmplementation file for Threads.h
* CS-325-400-W17	Project 4		17 March 2017
* Jonathan Horton	hortonjo@oregonstate.edu
* Chris Kearns		kearnsc@oregonstate.edu
* Dustin Pack		packdu@oregonstate.edu
******************************************************************************/
#include "Threads.h"

// Called from main() once thread setup and partitioning routines complete.
void Thread::start() {
	int temp;
	if ((temp = pthread_create(&tid, NULL, &Thread::exec, this)) != 0) {
		cout << strerror(temp) << "/n";
	}
}

// Each thread is run from here.
void *Thread::exec(void *thread) {
	((Thread *)thread)->run();
	return NULL;
}

// Called from Thread::exec() above.
void Thread::run() {
	int result = aThread->findBestPath(startNode);
	aThread->allPathLengths[TSPid][0] = startNode;
	aThread->allPathLengths[TSPid][1] = result;
	pthread_exit(0);
}

// Wait for thread termination status.
void Thread::join() {
	void *threadStatus;
	int temp = pthread_join(tid, &threadStatus);
	if (temp) {
		cout << "ERROR from tid " << TSPid << ". Error returned from "
			<< "pthread_join() is " << temp << "\n";
		exit(1);
	}
}
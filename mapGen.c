/*
 * This quick little c program will generate
 * a randomized map file for testing different
 * map sizes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[]) {
	// Check usage
	if (argc != 2) {
		fprintf(stderr, "USAGE: %s mapsize\n", argv[0]);
		exit(1);
	}

	// Check argument
	int maxCities = atoi(argv[1]);
	if (maxCities <= 0) {
		fprintf(stderr, "mapsize must be an integer > 0\n");
		exit(1);
	}

	// Seed RNG
	srand(time(NULL));

	// Generate random map up to maxCities
	int counter;
	for (counter = 0; counter < maxCities; counter++) {
		printf("%d %d %d\n", 
			counter, rand() % 20000, rand() % 20000);
	}


	return 0;
}

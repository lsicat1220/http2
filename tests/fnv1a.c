#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main() {
	srand(time(NULL));
	const int total = 100;
	const int map_size = round(total / 0.75);
	int collisions = 0;
	unsigned long hash;
	unsigned char map[map_size];
	memset(map, 0, map_size);
	size_t size;
	for (int i = 0; i < total; i++) {
		size = rand() % 50;
		char* str = (char*) malloc(sizeof(char) * size);
		for (int j = 0; j < size; j++) {
			str[j] = rand() % 256;
		}
		hash = Hash(str, size) % map_size;	
		if (map[hash] != 0) {
			collisions += 1;
		}
		map[hash] += 1;
		free(str);
	}
	printf("Amount of collisions: %d\n", collisions);
	printf("Final count\n");
	for (int i = 0; i < map_size; i++) {
		printf("%d, ", map[i]);
	}
	printf("\n");
	return 0;
}


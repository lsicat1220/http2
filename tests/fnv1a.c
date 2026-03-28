#include "../include/slice.h"
#include "../include/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main() {
	srand(time(NULL));
	const int total = 100;
	const int map_size = round(total / 0.75);
	Slice keys[total];
	memset(keys, 0, sizeof(keys));
	int collisions = 0;
	mapNode* map[map_size];
	memset(map, 0, sizeof(mapNode*) * map_size);
	size_t size;
	printf("Starting loop\n");
	for (int i = 0; i < total; i++) {
		size = (rand() % 49) + 1;
		char* key = (char*) malloc(sizeof(char) * size);
		char* value = (char*) malloc(sizeof(char) * 64);
		snprintf(value, 64, "This is the node created in index %d\n", i);
		keys[i].start = key;
		keys[i].len = size;
		mapNode* node = (mapNode*) malloc(sizeof(mapNode));
		node->key = key;
		node->value = value;
		node->key_size = size;
		node->value_size = 64;
		for (int j = 0; j < size; j++) {
			key[j] = rand() % 256;
		}
		if (MapSet(map, map_size, node) < 0) {
			collisions++;
		}
	}
	printf("Amount of collisions: %d\n", collisions);
	printf("Filled indices\n");
	for (int i = 0; i < map_size; i++) {
		int output = 0;
		if (map[i]) {
			output = 1;
		} 		
		printf("%d ", output);
	}
	printf("\n\n");
	printf("Checking indices:\n");
	for (int i = 0; i < 10; i++) {
		int random_index = rand() % (total);
		Slice key = keys[random_index];
		printf("Checking random index %d\n", random_index);
		mapNode* node = MapGet(map, map_size, key.start, key.len);
		if (node) {
			fwrite(node->value, 1, node->value_size, stdout);
		} else {
			printf("Node not found\n");
		}
	}
	return 0;
}


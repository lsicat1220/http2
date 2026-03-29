#include <stdlib.h>
#ifndef MAP_H
#define MAP_H
typedef struct {
	void* key;
	void* value;
	size_t key_size;
	size_t value_size;
} mapNode;

unsigned int Hash(void* input, size_t size);

int MapSet(mapNode** map, size_t map_size, mapNode* node);

mapNode* MapGet(mapNode** map, size_t map_size, void* key, size_t key_size);
#endif

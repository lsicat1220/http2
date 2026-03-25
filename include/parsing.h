#include "../include/receiving.h"
#ifndef PARSING_H

typedef struct {
	char* start;
	int len;
} Slice;

typedef struct {
	void* key;
	void* value;
	size_t key_size;
	size_t value_size;
} mapNode;

int GetSlice(bufState* buf_state, Slice* slice, const char* delim, const int delim_len);

unsigned int Hash(void* input, size_t size);

int MapSet(mapNode** map, size_t map_size, mapNode* node);

mapNode* MapGet(mapNode** map, size_t map_size, void* key, size_t key_size);



#endif

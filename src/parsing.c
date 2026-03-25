#include <stdio.h>
#include <string.h>
#include "../include/parsing.h"

int GetSlice(bufState* buf_state, Slice* slice, const char* delim, const int delim_len) {
	char* start = buf_state->buffer + buf_state->offset;
	int max_len = buf_state->capacity - buf_state->offset;
	char* slice_end = TheMemmem(delim, start, delim_len, max_len);
	if (!slice_end) {
		fprintf(stderr, "ERROR: No delimiter found when creating slice\n");
		return -1;
	}
	int slice_len = slice_end - start;
	slice->start = start;
	slice->len = slice_len;
	buf_state->offset += slice_len + delim_len;
	return 0;
}

unsigned int Hash(void* input, size_t size) {
	const unsigned int FNV_prime = 0x01000193;
	const unsigned int FNV_offset_basis = 0x811c9dc5;
	unsigned int hash = FNV_offset_basis;
	unsigned char* data = input;
	for (int i = 0; i < size; i++) {
		hash ^= data[i];
		hash *= FNV_prime;
	}	
	return hash;
}

int MapSet(mapNode** map, size_t map_size, mapNode* node) {
	unsigned int index = Hash(node->key, node->key_size) % map_size;
	unsigned int initial_index = index;
	unsigned int next_index;
	while (map[index]) {
		next_index = (index + 1) % map_size;
		if (next_index == initial_index) {
			fprintf(stderr, "ERROR: Unable to find free index in map\n");
			return -1;
		}
		if (map[index]->key_size == node->key_size && !memcmp(map[index]->key, node->key, node->key_size)) {
			break;
		}
		index = next_index;
	}
	map[index] = node;
	return 0;
}

mapNode* MapGet(mapNode** map, size_t map_size, void* key, size_t key_size) {
	unsigned int index = Hash(key, key_size) % map_size;
	unsigned int initial_index = index;
	while (map[index] != NULL) {
		if (map[index]->key_size == key_size && !memcmp(map[index]->key, key, key_size)) {
			return map[index];
		}
		index = (index + 1) % map_size;
		if (index == initial_index) {
			break;
		}
	}
	return NULL;
}

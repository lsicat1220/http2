#include <stdio.h>
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
	const unsigned long FNV_offset_basis = 0xcbf29ce484222325;
	const unsigned long FNV_prime = 0x100000001b3;
	unsigned long hash = FNV_offset_basis;
	char* data = input;
	int mask = 1;
	char byte_of_data;
	for (int i = 0; i < size; i++) {
		byte_of_data = (*data >> i) & mask;
		hash ^= byte_of_data;
		hash *= FNV_prime;
	}	
	return hash;
}

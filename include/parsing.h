#include "../include/receiving.h"
#ifndef PARSING_H

typedef struct {
	char* start;
	int len;
} Slice;


int GetSlice(bufState* buf_state, Slice* slice, const char* delim, const int delim_len);

unsigned int Hash(void* input, size_t size);


int SplitSlice(Slice* input, Slice* outputs, int num_outputs, char* delim, size_t delim_len);


#endif

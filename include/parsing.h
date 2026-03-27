#include "../include/receiving.h"
#ifndef PARSING_H

typedef struct {
	char* start;
	int len;
} Slice;

void InitSlice(Slice* new_slice, char* input);

int GetSlice(bufState* buf_state, Slice* slice, const char* delim, const int delim_len);


int SplitSlice(Slice* input, Slice* outputs, int num_outputs, char* delim, size_t delim_len);

void TrimSlice(Slice* input);

#endif

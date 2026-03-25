#include "../include/receiving.h"
#ifndef PARSING_H

typedef struct {
	char* start;
	int len;
} Slice;

int GetSlice(bufState* buf_state, Slice* slice, const char* delim, const int delim_len);

#endif

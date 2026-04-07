#include "../include/slice.h"
#ifndef PARSING_H
#define PARSING_H
typedef struct {
	int method;
	int version;
	char* file;
} reqParams;

int GetMethod(Slice* method);

int GetVersion(Slice* version);

int SliceToStr(char* dest, Slice* slice, size_t dest_len);

#endif 

#include <stdio.h>
#include <string.h>
#include "../include/slice.h"
#include "../include/parsing.h"

int GetMethod(Slice* method) {
	switch (method->len) {
		case 3:
			if (!memcmp(method->start, "GET", 3)) {
				return 0;
			} 			
			break;
	}
	return -1;
}
// HTTP/1.1
int GetVersion(Slice *version) {
	if (version->len != 8 || memcmp(version->start, "HTTP/1.", 7)) {
		return -1;
	}
	char ver = version->start[7];
	switch (ver) {
		case '0':
			return 0;
		case '1':
			return 1;
	}
	return -1;
}

int SliceToStr(char* dest, Slice* slice, size_t dest_len) {
	if (dest_len < slice->len) {
		fprintf(stderr, "ERROR: Size of destination less than slice length\n");
		return -1;
	}
	memcpy(dest, slice->start, slice->len);
	dest[slice->len] = 0;
	return 0;
}



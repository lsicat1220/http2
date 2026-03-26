#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
// This is a custom version of memmem() for compatibility with systems without it.
// Other than the order of parameters, the functionality should be identical.
void* TheMemmem(const void* needle, const void* haystack, size_t needlelen, size_t haystacklen) {
	// Error handling
	if (haystacklen == 0 || needlelen == 0) {
		return NULL;
	} else if (needlelen > haystacklen) {
		return NULL;
	}
	// Converting the given pointers into char pointers
	const unsigned char* target = needle;
	const unsigned char* start = haystack;
	// The last byte checked is a needle length back from the actual end
	const unsigned char* end = start + haystacklen - needlelen + 1;
	// Search for the first character in the length between the start and effective end
	const unsigned char* ptr = memchr(start, target[0], end - start);
	int compare_status;
	void* ret = NULL; // The pointer to the found needle 
	while (ptr && !ret) {
		compare_status = memcmp(ptr, target, needlelen);
		if (!compare_status) {
			ret = (void*) ptr;
		}	else {
			ptr = memchr(ptr + 1, *target, end - (ptr + 1));
		} 	
	}
	return ret;
}

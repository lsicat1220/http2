#include "../include/slice.h"
#include <stdio.h>

int main() {
	char tests[][64] = {"   this is a test   ", "the second test   ", "this shouldn't change", "        "};
	int num_tests = sizeof(tests) / 64;
	Slice slice;
	for (int i = 0; i < num_tests; i++) {
		InitSlice(&slice, tests[i]);
		printf("Initial string: \"");
		fwrite(slice.start, 1, slice.len, stdout);
		printf("\"\n");
		TrimSlice(&slice);
		printf("Trimmed string: \"");
		fwrite(slice.start, 1, slice.len, stdout);
		printf("\"\n");
	}
}

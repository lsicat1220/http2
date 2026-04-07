#include <stdio.h>
#include <string.h>
#include "../include/slice.h"
#include "../include/receiving.h"
#include "../include/utils.h"

void InitSlice(Slice* new_slice, char* input) {
	new_slice->start = input;
	new_slice->len = strlen(input);
}

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

int SplitSlice(Slice* input, Slice* outputs, int num_outputs, char* delim, size_t delim_len) {
	int occurences = 0;
	char* next_delim = NULL;
	char* cursor = input->start;
	int len = 0;
	int remaining_bytes = input->len;
	while (remaining_bytes > 0 && occurences < num_outputs) {
		next_delim = TheMemmem(delim, cursor, delim_len, remaining_bytes);
		if (!next_delim || occurences == num_outputs - 1) {
			len = remaining_bytes;	
		} else {
			len = next_delim - cursor;
		}
		remaining_bytes -= len + delim_len;
		outputs[occurences].start = cursor;
		outputs[occurences].len = len;
		occurences++;
		cursor = next_delim + delim_len;
	}
	return occurences;
}
// 0 1 2 3 4 5 6 7 8 9
void TrimSlice(Slice* input) {
	char* cursor = input->start;
	while (*cursor == ' ' && cursor - input->start + 1 < input->len) {
		cursor++;	
		input->len--;
	}
	input->start = cursor;

	cursor = input->start + input->len - 1;
	while (*cursor == ' ' && cursor - input->start > 0) {
		cursor--;
		input->len--;
	}

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

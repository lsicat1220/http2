#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receiving.h"

typedef struct sectionTest {
	const char* data;
	int chunks[10];
	int current_chunk;
	int num_chunks;
	int bytes_read;
} sectionTest;


sectionTest tests[] = {
	{
		.data = "blah blah blah this is a test\r\n\r\nyadayadayada",
		.chunks = {18, 18, 3},
		.current_chunk = 0,
		.num_chunks = 3,
		.bytes_read = 0
	},
	{
		.data = "this is another test hahahahahahah",
		.chunks = {2,4,9},
		.current_chunk = 0,
		.num_chunks = 3,
		.bytes_read = 0
	},
	{
		.data = "GET / HTTP/1.1\r\nHeader: blah\r\nHeader: yada\r\nHeader:yeah\r\n",
		.chunks = {2, 13, 35},
		.num_chunks = 3,
		.current_chunk = 0,
		.bytes_read = 0
	},
	{
		.data = "",
		.chunks = {0},
		.num_chunks = 0,
		.bytes_read = 0,
	},
	{
		.data = "GET / HTTP/1.1\r\nHeader: blah\r\nHeader: yada\r\nHeader:yeah\r\n",
		.chunks = {strlen( "GET / HTTP/1.1\r\nHeader: blah\r\nHeader: yada\r\nHeader:yeah\r\n")},
		.num_chunks = 3,
		.current_chunk = 0,
		.bytes_read = 0
	}
};


int __wrap_read(int testnum, char* ptr, int max_size) {
	sectionTest* test = &tests[testnum];
	const char* data = test->data;
	int cpysize;
	if (test->current_chunk > test->num_chunks) {
		cpysize = 0;
	} else if (max_size > test->current_chunk) {
		cpysize = test->chunks[test->current_chunk];	
	} else {
		cpysize = max_size;
	}
	memcpy(ptr, data + test->bytes_read, cpysize);
	test->bytes_read += cpysize;
	test->current_chunk++;
	return cpysize;	
}


int main() {
	char buffer[8192];
	bufState bufstate = {
		buffer,
		8192,
		0,
		0
	};
	for (int i = 0; i < sizeof(tests) / sizeof(sectionTest); i++) {
		printf("\r----Test %d----\n", i);
		int status = ReceiveSection(i, &bufstate, "\r\n", 2);
		if (status < 0) {
			HandleReadError(status);
		} else {
			printf("Read success!\n");
		}
		printf("Full buffer {\n");
		fwrite(buffer, 1, bufstate.full_len, stdout);
		printf("\n}\n");
		printf("Data after next_start {\n");
		fwrite(buffer + bufstate.next_start, 1, bufstate.full_len - bufstate.next_start, stdout);
		printf("\n}\n");
		memset(buffer, 0, 8192);
	}
}

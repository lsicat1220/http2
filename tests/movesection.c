#include <stdio.h>
#include <string.h>
#include "../include/receiving.h"

int main() {
	char buffer[1024];
	char dest[128];
	strcpy(buffer, "Hello goodbye yadayadayada good news to you hello hahahahahaha end");
	bufState buf = {
		.buffer = buffer,
		.capacity = 1024,
		.used = strlen(buffer),
		.offset = 14
	};
	printf("Intended next start: %s\n", buffer + buf.offset);
	int seclen = MoveSection(&buf, dest, 128);
	printf("Section moved: \n");
	fwrite(dest, sizeof(char), seclen, stdout);
	printf("\nRemaining data:\n");
	fwrite(buffer, sizeof(char), buf.offset, stdout);
	return 0;
}

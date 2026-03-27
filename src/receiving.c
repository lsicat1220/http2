#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include "../include/receiving.h"
#include "../include/utils.h"

#define NODEBUG

// This function edits a buf_state struct, which includes the buffer itself,
// the total amount of the bytes read, and the length of the most recent section received.

// next_start refers to amount of bytes from the start of the buffer the 
// next call of this function should start at.
int ReadUntil(int fd, bufState* buf_state, char* target, int target_size) {
	int newRead;
	int offset;
	int status = 0;
	char* buffer = buf_state->buffer;
	int* used = &(buf_state->used);
	const size_t capacity = buf_state->capacity;
	char* pos = buffer + buf_state->offset;
	char* occurrence = TheMemmem(target, pos, target_size, *used - (pos - buffer));
	while (!occurrence && *used < capacity) {
		newRead = read(fd, buffer + *used, capacity - *used);
		if (newRead < 0) {
			status = ERR_SOCKET_FAILED; //read failed
			break;
		} else if (newRead == 0) {
			if (*used) {
				status = ERR_READ_INTERRUPTED; //read interrupted or ended prematurely
			} else {
				status = ERR_NO_DATA; //read ended with no data
			}
			break;
		}
		*used += newRead;
		if (pos - buffer < target_size - 1) {
			offset = pos - buffer;
		} else {
			offset = target_size - 1;
		}
		occurrence = TheMemmem(target, pos - offset, target_size, newRead + offset);
		pos = buffer + *used;
	} 
#ifdef DEBUG
	printf("Data read:\n%s\n", (char*) buffer);
#endif
	if (status == 0 && !occurrence) {
		status = ERR_BUFFER_FULL; //ran out of space, no headers
	}	
	return status;  
}

void HandleReadError(enum receiveStatus status) {
	if (status == -1) {
		perror("ERROR");
	} else if (status == -2) {
		fprintf(stderr, "ERROR: Request interrupted or ended prematurely\n");
	} else if (status == -3) {
		fprintf(stderr,"ERROR: Read ended with no data\n");
	} else if (status == -4) {
		fprintf(stderr, "ERROR: Ran out of space with incomplete headers\n");
	} else {
		fprintf(stderr, "ERROR: Unknown error");
	}
}


// This is a simple function that takes data stored in bufState to move the most recent section to a new string
int MoveSection(bufState* bufState, void* dest, size_t dest_len) {
	int sectionLen = bufState->offset - 1;
	if (sectionLen > dest_len) {
		fputs("ERROR: Attempt to move data to a smaller buffer", stderr);
		return 0;
	}
	memcpy(dest, bufState->buffer, sectionLen);
	return sectionLen;
}

// This function shifts the buffer such that the beginning of the buffer is now 
// the position refered to by bufState.offset
int CompactBuffer(bufState* buf) {
	int len = buf->used -= buf->offset;
	memmove(buf->buffer, buf->buffer + buf->offset, len);
	buf->used = len;
	buf->offset = 0;	
	memset(buf->buffer + len, 0, buf->capacity - len);
	return 0;
}

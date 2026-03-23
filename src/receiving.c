#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include "../include/receiving.h"

#define NODEBUG

// This function returns the bytes read up to the first character of the target
// The full amount of bytes read is stored in the bytes_read pointer
int ReceiveSection(int fd, bufState* buf_state, char* target, int target_size) {
	int newRead;
	int offset;
	int status = 0;
	char* buffer = buf_state->buffer;
	int* bytes_read = &(buf_state->full_len);
	int buf_size = buf_state->buf_size;
	char* ptr = buffer + buf_state->next_start;
	char* occurrence = TheMemmem(target, ptr, target_size, *bytes_read - (ptr - buffer));
	while (!occurrence && *bytes_read < buf_size) {
		newRead = read(fd, buffer + *bytes_read, buf_size - *bytes_read);
		if (newRead < 0) {
			status = ERR_SOCKET_FAILED; //read failed
			break;
		} else if (newRead == 0) {
			if (*bytes_read) {
				status = ERR_READ_INTERRUPTED; //read interrupted or ended prematurely
			} else {
				status = ERR_NO_DATA; //read ended with no data
			}
			break;
		}
		*bytes_read += newRead;
		if (ptr - buffer < target_size - 1) {
			offset = ptr - buffer;
		} else {
			offset = target_size - 1;
		}
		occurrence = TheMemmem(target, ptr - offset, target_size, newRead + offset);
		ptr = buffer + *bytes_read;
	} 
#ifdef DEBUG
	printf("Data read:\n%s\n", (char*) buffer);
#endif
	if (status == 0) {
		if (!occurrence) {
			status = ERR_BUFFER_FULL; //ran out of space, no headers
		} else {
			buf_state->next_start = (occurrence + target_size) - buffer;
		}
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

int MoveSection(bufState* bufState, void* dest, size_t dest_len) {
	int sectionLen = bufState->next_start - 1;
	if (sectionLen > dest_len) {
		fputs("ERROR: Attempt to move data to a smaller buffer", stderr);
		return 0;
	}
	memcpy(dest, bufState->buffer, sectionLen);
	return sectionLen;
}

int CompactBuffer(bufState* buf, size_t len) {
	memmove(buf->buffer, buf->buffer + buf->next_start, len);
	buf->next_start = 0;	
	memset(buf->buffer + len, 0, buf->buf_size - len);
	return 0;
}

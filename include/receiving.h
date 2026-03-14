#include <stdlib.h>
#ifndef RECEIVE_H
#define RECEIVE_H 
enum contentType {
	HTML,
	CSS,
	JS,
	TXT,
	JSON,
	ICO,
	UNSUPPORTED,
};
enum methods {
	GET,
	POST,
};
enum receiveStatus {
	ERR_SOCKET_FAILED = -1,
	ERR_READ_INTERRUPTED = -2,
	ERR_NO_DATA = -3,
	ERR_BUFFER_FULL = -4
};
typedef struct bufState {
	char* buffer;
	int buf_size;
	int full_len;
	int next_start;
} bufState;
typedef struct reqParams {
	char host[256];
	int keepAlive;
	int method;
	int httpVersion;
	int contentLength;
	char filepath[256];
} reqParams;

int ReceiveSection(int fd, bufState* buf_state, char* target, int target_size);

void HandleReadError(enum receiveStatus status);

void* TheMemmem(const void* needle, const void* haystack, size_t needlelen, size_t haystacklen);

int MoveSection(bufState* bufState, void* dest, size_t dest_len);

int CompactBuffer(bufState* buf, size_t len);

#endif

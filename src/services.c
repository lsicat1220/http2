#include <stdio.h>
#include <unistd.h>
#include "../include/receiving.h"
#include "../include/services.h"
#include "../include/slice.h"
#define BUFFER_SIZE 8192

void HandleConnection(int clientSocket) {
	int keepAlive = 1;
	printf("\n\n----- CONNECTION ACCEPTED -----\n\n");
	char readBuffer[BUFFER_SIZE];
	char responseBuffer[BUFFER_SIZE];
	while (keepAlive) {
		bufState buffer_state = {readBuffer, BUFFER_SIZE, 0, 0};
		Slice request;
		int readStatus = ReadUntil(clientSocket, &buffer_state, "\r\n\r\n", 4);
		if (readStatus < 0) {
			HandleReadError(readStatus);
			break;
		}
		// by now the buffer should have all of the headers
		if (GetSlice(&buffer_state, &request, "\r\n", 2)) {
			break;
		}
		printf("Headers: \n");
		fwrite(request.start, request.len, sizeof(char), stdout);
		printf("\n\nThe Rest:\n");
		fwrite(buffer_state.buffer + buffer_state.offset, BUFFER_SIZE - buffer_state.offset, sizeof(char), stdout);
		keepAlive = 0;
	}
	snprintf(responseBuffer, BUFFER_SIZE, "HTTP/1.0 200 OK\r\n\r\n");
	write(clientSocket, responseBuffer, 19);
	printf("\n\n----- CONNECTION CLOSED -----\n\n");
	close(clientSocket);
	snprintf(responseBuffer, BUFFER_SIZE, "HTTP/1.0 200 OK\r\n\r\n");
	write(clientSocket, responseBuffer, 19);
	close(clientSocket);
}

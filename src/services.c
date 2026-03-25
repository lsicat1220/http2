#include <stdio.h>
#include <unistd.h>
#include "../include/receiving.h"
#include "../include/services.h"
#define BUFFER_SIZE 8192

void HandleConnection(int clientSocket) {
	int keepAlive = 1;
	printf("\n\n----- CONNECTION ACCEPTED -----\n\n");
	char readBuffer[BUFFER_SIZE];
	char responseBuffer[BUFFER_SIZE];
	while (keepAlive) {
		bufState buffer_state = {readBuffer, BUFFER_SIZE, 0, 0};
		char method[1024];
		char headers[1024];
		int readStatus = ReadUntil(clientSocket, &buffer_state, "\r\n", 2); 
		if (readStatus < 0) {
			HandleReadError(readStatus);
			break;
		}
		int methodlen = MoveSection(&buffer_state, method, 1024);	
		CompactBuffer(&buffer_state);
		readStatus = ReadUntil(clientSocket, &buffer_state, "\r\n\r\n", 4);
		if (readStatus < 0) {
			HandleReadError(readStatus);
			break;
		}
		int headerslen = MoveSection(&buffer_state, headers, buffer_state.used - 1024);
		CompactBuffer(&buffer_state);
		printf("Method:\n");
		fwrite(method, sizeof(char), methodlen, stdout);
		printf("\n\n");
		printf("Headers:\n");
		fwrite(headers, sizeof(char), headerslen, stdout);
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

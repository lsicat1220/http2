#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include "receiving.h"

#define PORT 6767
#define BUFFER_SIZE 8192
#define DEBUG

int main() {
	printf("Starting server...\n");
	int keepAlive = 1;

	char readBuffer[BUFFER_SIZE];
	char responseBuffer[BUFFER_SIZE];

	struct sockaddr_in addr;
	int newSocket = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 250000; 

	int opt = 1;
	setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 
 
	if (bind(newSocket, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
		printf("ERROR: Binding failed\n");
		return -1;
	}

	listen(newSocket, 20);
	printf("Now listening on port %d...\n", PORT);

	while(1) {
		struct sockaddr_in newAddr;
		socklen_t newAddrSize = sizeof(newAddr);
		int clientSocket = accept(newSocket, (struct sockaddr*) &newAddr, &newAddrSize);
		setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)); 
		setsockopt(clientSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)); 
		if (clientSocket < 0) {
			printf("NONFATAL ERROR: Unable to accept socket; discarding...\n");
			continue;
		}
		printf("\n\n----- CONNECTION ACCEPTED -----\n\n");
		keepAlive = 1;
		while (keepAlive) {
			bufState buffer_state = {readBuffer, BUFFER_SIZE, 0, 0};
			int readStatus = ReceiveSection(clientSocket, &buffer_state, "\r\n", 2); 
			char section1[1024];
			if (readStatus < 0) {
				HandleReadError(readStatus);
				break;
			}
			int sectionlen = MoveSection(&buffer_state, section1, 1024);	
			CompactBuffer(&buffer_state, buffer_state.full_len - sectionlen);
			printf("Section:\n");
			fwrite(section1, sizeof(char), sectionlen, stdout);
			printf("\n\n");
			printf("Remaining buffer:\n");
			fwrite(buffer_state.buffer, sizeof(char), buffer_state.next_start - 1, stdout);
			printf("\n");
			printf("Starting request parsing...\n");
			keepAlive = 0;
		}

		snprintf(responseBuffer, BUFFER_SIZE, "HTTP/1.0 200 OK\r\n\r\n");
		write(clientSocket, responseBuffer, 19);
		printf("\n\n----- CONNECTION CLOSED -----\n\n");
		close(clientSocket);
	}
}

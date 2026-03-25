#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include "../include/receiving.h"
#include "../include/services.h"

#define PORT 6767
#define BUFFER_SIZE 8192
#define DEBUG

int main() {
	printf("Starting server...\n");

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
		HandleConnection(clientSocket);	
	}
}

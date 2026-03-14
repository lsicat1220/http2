#include <stdio.h>
#include <stdlib.h>

int DummyResponse(char* response) {
	return snprintf(response, 20, "HTTP/1.0 200 OK\r\n\r\n");
}


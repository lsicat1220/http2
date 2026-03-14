#include <stdio.h>
#include <string.h>
#include "parsing.h"
#include <stdlib.h>
#include <unistd.h>

typedef struct MethodLinePtrs {
	char* link;
	char* version;
} MethodLinePtrs;

int ReadMethod(char* buffer, int length, reqParams params) {
	if (length > 3 && strncmp(buffer, "GET", 3)) {
		params.method = GET;
		return 0;
	} else {
		return -1; 
	}
}

int ValidateLink(char* link, int length, reqParams params) {
	char workingDir[64];
	getcwd(workingDir, 64);	
	if (TheMemmem("../", link, 3, length)) {
		return -1;
	} 
	return 0;
}

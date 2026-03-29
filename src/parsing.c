#include <stdio.h>
#include <string.h>
#include "../include/slice.h"
#include "../include/parsing.h"

int GetMethod(Slice* method) {
	switch (method->len) {
		case 3:
			if (!memcmp(method->start, "GET", 3)) {
				return 0;
			} 			
			break;
	}
	return -1;
}

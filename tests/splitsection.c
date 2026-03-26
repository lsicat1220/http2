#include <stdio.h>
#include "../include/parsing.h"
#include <string.h>

typedef struct {
	Slice* target;
	Slice* delimiter;
	Slice* outputs;
	int num_sections;
} Test;
Test* InitTest(char* str, char* delim, int num_sections) {
	Test* new_test = (Test*) malloc(sizeof(Test));
	Slice* target = (Slice*) malloc(sizeof(Slice));
	Slice* delimiter = (Slice*) malloc(sizeof(Slice));
	Slice* outputs = (Slice*) malloc(sizeof(Slice) * num_sections);
	target->start = str;
	target->len = strlen(str);
	delimiter->start = delim;
	delimiter->len = strlen(delim);
	new_test->target = target;
	new_test->delimiter = delimiter;
	new_test->outputs = outputs;
	new_test->num_sections = num_sections;
	return new_test;
}


int main() {
	char str1[] = "Hello world I am writing a program";
	char str2[] = "the;delimiter;is;a;semicolon";
	char str3[] = "there.will.not.be.enough.fields";
	char str4[] = "multispacecharacterspacedelim";
	Test* tests[4];
	tests[0] = InitTest(str1, " ", 7);
	tests[1] = InitTest(str2, ";", 5);
	tests[2] = InitTest(str3, ".", 2);
	tests[3] = InitTest(str4, "space", 3);
	for (int i = 0; i < 4; i++) {
		printf("Slice %d: ", i + 1);
		fwrite(tests[i]->target->start, 1, tests[i]->target->len, stdout);
		printf("\n");

		int num_slices = SplitSlice(tests[i]->target, tests[i]->outputs, tests[i]->num_sections, tests[i]->delimiter->start, tests[i]->delimiter->len);
		printf("Number of sections: %d\n", num_slices);	
		for (int j = 0; j < num_slices; j++) {
			fwrite(tests[i]->outputs[j].start, 1, tests[i]->outputs[j].len, stdout);
			printf("\n");
		}
	}
}

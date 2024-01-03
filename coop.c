/*
 * Author: Michael Henry Jennings
 * v0.0 (skeleton code and specifications): 1/2/2024
 * v0.1 (expanded specifications): 1/3/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char** handle_class(char *name, char **class) {
	// find data members & define struct
	// for each function definition, replace with non-objective
	// identify & amend calls to such functions within class
}

char** deobjectify() {
	// find & amend constructed objects and operations on them
}

char** declassify(char *text) {
	// find instances of "class" surrounded by whitespace
	// find braces, read class name, and call handle_class
	// pass the list of all defined classes to deobjectify
}

char *get_extension(char* filename) {
	// find last character, storing last period
	// return such string at minimum possible size
}

int main(int argc, char *argv[]) {
	// check extensions (.coop)
	// call subroutines to alter the file's text
	// create and write to .c file of same prefix
	printf(read("coop_readme.txt"));
}

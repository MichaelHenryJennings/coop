/*
 * Author: Michael Henry Jennings
 * v0.0 (skeleton code and specifications): 1/2/2024
 *
 */

int main(int argc, char *argv[]) {
	// check extensions (.coop)
	// call subroutines to alter the file's text
	// create and write to .c file of same prefix
}

char* read(char *filename) {
	// read and return contents of a file
}

char* write(char* text, char* filename) {
	// write text to a file
}
char** declassify(char *contents) {
	// find instances of "class" surrounded by whitespace
	// find braces, read class name, and call handle_class
	// pass the list of all defined classes to deobjectify
}

char** handle_class(char *name, char **class) {
	// find data members & define struct
	// for each function definition, replace with non-objective
	// identify & amend calls to such functions within class
}

char** deobjectify() {
	// find & amend constructed objects and operations on them (checking within nearest set of braces)
}

char *get_extension(char* filename) {
	// find last character, storing last period
	// return such string at minimum possible size
}

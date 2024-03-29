/*
 * Author: Michael Henry Jennings
 * v0.0 (skeleton code and specifications): 1/2/2024
 * v0.1 (expanded specifications): 1/3/2024
 * v0.2 (some features working): 1/7/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define CLASS "class"
#define STRUCT "struct"
#define THIS "this"

char* read(FILE *file) {
	// read and return contents of a file
	unsigned int length = 0;
	unsigned int allocated = 16;
	char* start = malloc(allocated * sizeof(char));
	char* current = start;
	while ((*(current++) = fgetc(file)) != EOF) {
		if (++length == allocated) {
			start = realloc(start, (allocated *= 2) * sizeof(char));
			current = start + length;
		}
	}
	start = realloc(start, length * sizeof(char));
	*(start + length) = '\0';
	return start;
}

int letter(char character) {
	return isalpha(character) || character == '_';
}

char* read_word(char *text) {
	char *start = text;
	int len = 1;
	while (letter(*(++text))) {
		len++;
	}
	char temp = *text;
	*text = '\0';
	char *word = malloc(len * sizeof(char));
	strcpy(word, start);
	*text = temp;
	return word;
}

int count_instance_variables(char *text) {
	int count = 0;
	while (*text != ')') {
		if (*text == ',') {
			count++;
		}
		text++;
	}
	return ++count;
}

int function(char *text) {
	while (letter(*text)) {
		text++;
	}
	while (isspace(*text)) {
		text++;
	}
	return *text == '(';
}

int function_declaration(char *text) {
	while (letter(*text)) {
		text++;
	}
	while (isspace(*text)) {
		text++;
	}
	if (*(text++) != '(') {
		return 0;
	}
	while (*(text++) != ')') {
		text++;
	}
	while (isspace(*text)) {
		text++;
	}
	if (*text != '{') {
		return 0;
	}
	return 1;
}

char *get_extension(char* filename) {
	// find last character, storing last period
	char *start = NULL;
	while (*filename != '\0') {
		if (*(filename++) == '.') {
			start = filename;
		}
	}
	// return pointer to start of extension
	if (start == NULL) {
		return NULL;
	}
	return start;
}

int matches(char* text, char* pattern) {
	while(*pattern != '\0') {
		if (*(pattern++) != *(text++)) {
			return 0;
		}
	}
	return isspace(*text);
}

char* process_chain(char *text, FILE *file, char** instance_variables, int num_instance_variables) {
	char *start = text;
	char *vacancy = text;
	int was_dot = 0;
	while (letter(*text) || *text == '.' || *text == '(') {
		if (*text == '.') {
			was_dot = 1;
		}
		if (*(text++) == '(') {
			if (was_dot) {
				vacancy = text;
			}
			int counter = 1;
			while (counter > 0) {
				if (*text == '(') {
					counter++;
				}
				if (*(text++) == ')') {
					counter--;
				}
			}
		}
	}
	char* function = vacancy;
	while (*function != '.' && function >= start) {
		function--;
	}
	char temp = *vacancy;
	*vacancy = '\0';
	fputs(++function, file);
	*vacancy = temp;
	if (function != start) {
		temp = *(--function);
		*function = '\0';
		process_chain(start, file, instance_variables, num_instance_variables);
		*function = temp;
	} else {
		char *word = read_word(start);
		for (int j = 0; j < num_instance_variables; j++) {
			if (!strcmp(word, instance_variables[j])) {
				fputs(THIS, file);
				fputc('.', file);
			}
		}
		free(word);
	}
	while (isspace(*vacancy)) {
		fputc(*(vacancy++), file);
	}
	if (*vacancy != ')' && vacancy != start) {
		fputs(", ", file);
	}
	while (vacancy < text) {
		if (*vacancy == '(') {
			while (*vacancy != ')') {
				if (letter(*vacancy)) {
					vacancy = process_chain(vacancy, file, instance_variables, num_instance_variables);
				} else {
					fputc(*(vacancy++), file);
				}
			}
		} else {
			fputc(*(vacancy++), file);
		}
	}
	return text;
}

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (!strcmp(get_extension(argv[i]), "coop")) { //.coop file
			// open .coop file for reading
			FILE *coopfile;
			if ((coopfile = fopen(argv[i], "r")) == NULL) {
				fprintf(stderr, "Error: .coop file cannot be read from.");
				return -1;
			}
			// open corresponding .c file for writing
			FILE *cfile;
			char* end_c_file = argv[i];
			while (*(++end_c_file + 3) != '\0') {}
			*end_c_file = '\0';
			if ((cfile = fopen(argv[i], "w")) == NULL) {
				fprintf(stderr, "Error: corresponding .c file cannot be written to.");
				return -2;
			}
			// setup
			int classes_allocated = 16;
			char **classes = malloc(sizeof(char*) * classes_allocated);
			int num_classes = 0;
			char *classname = NULL;
			char **instance_variables = malloc(sizeof(char*));
			int num_instance_variables = 0;
			int brace_level = 0;
			int in_constructor = 0;
			char* cooptext = read(coopfile);
			// scan entire file for coop syntax
			while (*cooptext != '\0') {
				if (letter(*cooptext)) {
					char *next_word = read_word(cooptext);
					if (!strcmp(next_word, CLASS)) {
						// found new class definition
						cooptext += strlen(CLASS);
						while (!letter(*cooptext)) {
							cooptext++;
						}
						// process class name
						classname = read_word(cooptext);
						if (classes_allocated == num_classes) {
							classes = realloc(*classes, classes_allocated *= 2);
						}
						classes[num_classes] = classname;
						num_classes++;
						while (letter(*cooptext)) {
							cooptext++;
						}
						while (*(cooptext++) != '(') {}
						// process instance variables; define struct
						fputs(STRUCT, cfile);
						fputc(' ', cfile);
						fputs(classname, cfile);
						fputs(" {\n", cfile);
						num_instance_variables = count_instance_variables(cooptext);
						for (int j = 0; j < num_instance_variables; j++) {
							fputc('\t', cfile);
							while (!letter(*cooptext)) {
								cooptext++;
							}
							while (letter(*cooptext)) {
								fputc(*cooptext, cfile);
								cooptext++;
							}
							while (!letter(*cooptext)) {
								fputc(*cooptext, cfile);
								cooptext++;
							}
							instance_variables[j] = read_word(cooptext);
							while (letter(*cooptext)) {
								fputc(*cooptext, cfile);
								cooptext++;
							}
							fputs(";\n", cfile);
						}
						fputs("};\n\n", cfile);
						while (*(cooptext++) != '{') {}
						while (isspace(*cooptext)) {
							cooptext++;
						}
						brace_level++;
					} else {
						int match = 0;
						if (!function(cooptext)) {
							fflush(stderr);
							for (int i = 0; i < num_classes; i++) {
								if (!strcmp(next_word, classes[i])) {
									fputs(STRUCT, cfile);
									fputc(' ', cfile);
									match++;
									break;
								}
							}
						}
						if (brace_level && !match) {
							if (function_declaration(cooptext) && brace_level == 1) {
								if (strcmp(next_word, classname)) {
									// handle instance method
									fputs(next_word, cfile);
									cooptext += strlen(next_word);
									while (*cooptext != '(') {
										fputc(*(cooptext++), cfile);
									}
									fputc(*(cooptext++), cfile);
									fputs(STRUCT, cfile);
									fputc(' ', cfile);
									fputs(classname, cfile);
									fputc(' ', cfile);
									fputs(THIS, cfile);
									while (isspace(*cooptext)) {
										fputc(*(cooptext++), cfile);
									}
									if (*cooptext != ')') {
										fputs(", ", cfile);
									}
									while (*cooptext != '{') {
										fputc(*(cooptext++), cfile);
									}
								} else {
									// handle constructor
									fputs(STRUCT, cfile);
									fputc(' ', cfile);
									fputs(classname, cfile);
									fputc(' ', cfile);
									while (*cooptext != '{') {
										fputc(*(cooptext++), cfile);
									}
									fputc(*(cooptext++), cfile);
									brace_level++;
									fputc('\n', cfile);
									fputs(STRUCT, cfile);
									fputc(' ', cfile);
									fputs(classname, cfile);
									fputc(' ', cfile);
									fputs(THIS, cfile);
									fputc(';', cfile);
									in_constructor = 1;
								}
							} else {
								cooptext = process_chain(cooptext, cfile, instance_variables, num_instance_variables);
							}
						} else {
							cooptext = process_chain(cooptext, cfile, instance_variables, num_instance_variables);
						}
					}
					free(next_word);
				} else {
					if (brace_level) {
						if (*cooptext == '{') {
							brace_level++;
						} else if (*cooptext == '}') {
							if (--brace_level == 0) {
								for (int j = 0; j < num_instance_variables; j++) {
									free(instance_variables[j]);
								}
								free(instance_variables);
								num_instance_variables = 0;
								classname = NULL;
								cooptext++;
								continue;
							}
							if (brace_level == 1 && in_constructor) {
								fputs("return ", cfile);
								fputs(THIS, cfile);
								fputs(";\n", cfile);
								in_constructor = 0;
							}
						}
					}
					fputc(*(cooptext++), cfile);
				}
			}
			fclose(coopfile);
			fclose(cfile);
			// call subroutines to alter the file's text
			// create and write to .c file of same prefix
		}
		// ignore other file types
	}
	return 0;
}

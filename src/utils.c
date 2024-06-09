/* utillity functions */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define INITIAL_LINE_LENGTH 128

char* trim_whitespace(char *str) {
	char *end;

	/* Trim leading space */
	while(isspace((unsigned char)*str)){ 
		str++;
	}

	if(*str == 0){  /* All spaces? */
		return str;
	}

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)){ 
		end--;
	}

	/* Write new null terminator character */
	end[1] = '\0';

	return str;
}

char* read_line(FILE *file){
	char *line = malloc(INITIAL_LINE_LENGTH);
	int length = INITIAL_LINE_LENGTH;
	int pos = 0;
	int c;
	
	if (!line){
		return NULL;
	}

	while ((c = fgetc(file)) != EOF && c != '\n'){
		if (pos >= length - 1){
			length *= 2;
			line = realloc(line, length);
			if (!line){
				return NULL;
			}
		}

		line[pos++] = c;
	}
	
	if (c == EOF && pos == 0){
		free(line);
		return NULL;
	}

	line[pos] = '\0';
	return line;
}

/* Duplicates a string by dynamically allocating memory for the copy and returning it. */
char* str_duplicate(const char *s) {
	size_t len = strlen(s) + 1; /* Length of the source string plus null terminator. */
	char *dup = malloc(len); /* Allocate memory for the duplicate. */
	if (dup != NULL) {
		memcpy(dup, s, len); /* Copy the string into the new memory. */
	}
	return dup;
}

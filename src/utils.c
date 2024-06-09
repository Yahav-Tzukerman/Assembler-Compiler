/* utillity functions */
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define INITIAL_LINE_LENGTH 128

static char *strtok_last;

char* strtoken(char *str, const char *delim) {
	char *start;
	char *token;

	if (str != NULL) {
		strtok_last = str;
	}

	if (strtok_last == NULL) {
		return NULL;
	}

	/* Skip initial delimiters */
	start = strtok_last;
	while (*start && strchr(delim, *start)) {
		start++;
	}

	if (*start == '\0') {
		strtok_last = NULL;
		return NULL;
	}

	/* Find the end of the token */
	token = start;
	while (*strtok_last && !strchr(delim, *strtok_last)) {
		strtok_last++;
	}

	if (*strtok_last) {
		*strtok_last = '\0';
		strtok_last++;
	} else {
		strtok_last = NULL;
	}

	return token;
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

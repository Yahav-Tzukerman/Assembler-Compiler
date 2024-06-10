/* Manages error reporting throughout the assembly process.
** Provides a centralized function to report errors consistently.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define INITIAL_ERROR_CAPACITY 10

static Error *errors = NULL;
static int error_count = 0;
static int error_capacity = 0;
static bool error_flag = false;

const char *error_messages[] = {
	"File not found: %s",
	"Macro name missing.",
	"Macro name is not valid: %s",
	"Memory allocation failed.",
	/* Add more error messages as needed */
	"Unknown error."
};

void init_error_handling() {
	if (errors != NULL) {
        	free(errors);
	}
	error_count = 0;
	error_capacity = INITIAL_ERROR_CAPACITY;
	errors = (Error *)malloc(error_capacity * sizeof(Error));
	if (errors == NULL) {
		fprintf(stderr, "Failed to allocate memory for error handling.\n");
		exit(EXIT_FAILURE);
	}
	error_flag = false;
}

void add_error(ErrorCode code, const char *filename, int line, const char *detail) {

	if (error_count >= error_capacity) {
		error_capacity *= 2;
		errors = (Error *)realloc(errors, error_capacity * sizeof(Error));
		if (errors == NULL) {
			fprintf(stderr, "Failed to reallocate memory for error handling.\n");
			exit(EXIT_FAILURE);
		}
	}

	errors[error_count].code = code;
	sprintf(errors[error_count].filename, "%s", filename);
	errors[error_count].line = line;
	
	if (detail) {
		sprintf(errors[error_count].message, error_messages[code], detail);
	} else {
		strncpy(errors[error_count].message, error_messages[code], sizeof(errors[error_count].message) - 1);
		errors[error_count].message[sizeof(errors[error_count].message) - 1] = NULL_TERMINATOR;
	}
	error_count++;
	error_flag = true;
}

void print_errors() {
	int i;
	for (i = 0; i < error_count; i++) {
		fprintf(stderr, "Error in file %s at line %d: %s\n", errors[i].filename, errors[i].line, errors[i].message);
	}
}

bool has_errors() {
	return error_flag;
}

void free_errors() {
	if (errors != NULL) {
		free(errors);
		errors = NULL;
	}
}


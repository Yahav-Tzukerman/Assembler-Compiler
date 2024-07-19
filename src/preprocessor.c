/* Handles macro expansion and prepares the source code for parsing.
** Reads the input file, processes macros, and writes the expanded code to a temporary file.
*/
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "validations.h"
#include "error.h"

#define INITIAL_LINE_CAPACITY 100
#define LINE_LENGTH 81 /* length of a line is 80 + 1 for null trminator */

static Macro *macro_list = NULL;
static bool macros_present = false;

void add_macro(Macro *new_macro){
	new_macro->next = macro_list;
	macro_list = new_macro;
	macros_present = true;
}

Macro* find_macro(const char *name){
	Macro *current = macro_list;
	while (current != NULL){
		if(strcmp(current->name, name) == 0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void add_preprocessed_line(Context *context, const char *line) {
	if (context->line_count >= context->line_capacity) {
		context->line_capacity *= 2;
		context->preprocessed_lines = (char **)realloc(context->preprocessed_lines, context->line_capacity * sizeof(char *));
		if (context->preprocessed_lines == NULL) {
			add_error(ERR_MEMORY_ALLOCATION_FAILED, context->filename, context->line_number, NULL);
			return;
		}
	}
	context->preprocessed_lines[context->line_count] = str_duplicate(line);
	context->line_count++;
}

void expand_macros(FILE *input, Context *context){
	char *line;
	char *line_duplicate;
	char *token;
	Macro *macro;
	int i;
	bool skip_lines = false;

	context->line_number = 1;
	while ((line = read_line(input)) != NULL){
		line_duplicate = str_duplicate(line);
		token = strtok(line_duplicate, " \t\n");
		
		if (token != NULL){

			if (strcmp(token, "macr") == 0) {
				skip_lines = true;
			} else if (strcmp(token, "endmacr") == 0) {
				skip_lines = false;
				free(line_duplicate);
				free(line);
				context->line_number++;
				continue;
			}

			if (skip_lines) {
				free(line_duplicate);
				free(line);
				context->line_number++;
				continue;
			}

	
			macro = find_macro(token);
			if (macro != NULL) {
				for (i = 0; i < macro->line_count; i++){
					add_preprocessed_line(context, macro->lines[i]);
				}
			} else {
				add_preprocessed_line(context, line);
			}
		} else {
			add_preprocessed_line(context, "");
		}
		
		free(line_duplicate);
		free(line);
		context->line_number++;
	}	
}


bool process_macro_definition(FILE *input, char *name, Context *context){
	int i;
	Macro *new_macro;
	char *line;
	char *trimmed_line;

	if (!validate_macro_name(name)){
		add_error(ERR_MACRO_NAME_IS_NOT_VALID, context->filename, context->line_number, name);
		return false;
	}
	
	new_macro = (Macro *)malloc(sizeof(Macro));
	if (new_macro == NULL) {
		add_error(ERR_MEMORY_ALLOCATION_FAILED, context->filename, context->line_number, NULL);
		return false;
	}

	strncpy(new_macro->name, name, sizeof(new_macro->name));
	new_macro->lines = NULL;
	new_macro->line_count = 0;
	new_macro->next = NULL;

	while ((line = read_line(input)) != NULL){
		trimmed_line = trim_whitespace(line);
		if (strncmp(trimmed_line, "endmacr", 7) == 0){
			free(line);
			break;		
		}

		new_macro->line_count++;
		new_macro->lines = (char **)realloc(new_macro->lines, new_macro->line_count * sizeof(char *));
		if (new_macro->lines == NULL) {
			add_error(ERR_MEMORY_ALLOCATION_FAILED, context->filename, context->line_number, NULL);
			free(line);
			for (i = 0; i < new_macro->line_count - 1; i++) {
				free(new_macro->lines[i]);
			}
			free(new_macro->lines);
			free(new_macro);
			return false;
		}
		new_macro->lines[new_macro->line_count - 1] = line;
		context->line_number++;
	}

	add_macro(new_macro);
	return true;
}

void free_context(Context *context) {
	int i;
	for (i = 0; i < context->line_count; i++) {
		free(context->preprocessed_lines[i]);
	}
	free(context->preprocessed_lines);
	context->preprocessed_lines = NULL;
}

void free_macros() {
	int i;
	Macro *current = macro_list;
	Macro *next;

	while (current != NULL) {
		next = current->next;
		for (i = 0; i < current->line_count; i++) {
			free(current->lines[i]);
		}
		free(current->lines);
		free(current);
		current = next;
	}
	macro_list = NULL;
}

bool preprocess(const char *filename, Context *context){
	FILE *input;
	char *line;
	char *token;
	char *macro_name;
	bool success = true;

	context->filename = filename;
	context->line_number = 1;
	context->preprocessed_lines = (char **)malloc(INITIAL_LINE_CAPACITY * sizeof(char *));
	context->line_count = 0;
	context->line_capacity = INITIAL_LINE_CAPACITY;

	if (context->preprocessed_lines == NULL) {
		add_error(ERR_MEMORY_ALLOCATION_FAILED, filename, 0, NULL);
		return false;
	}

	input = fopen(filename, "r");
	if (!input) {
		add_error(ERR_FILE_NOT_FOUND, filename, 0, NULL);
		free(context->preprocessed_lines);
		return false;
	}

	/* First pass: check for macros and process definitions */
	while ((line = read_line(input)) != NULL) {
		token = strtok(line, " \t\n");
		if (token != NULL && strcmp(token, "macr") == 0) {
			macro_name = strtok(NULL, " \t\n");
			if (macro_name != NULL) {
				if(!process_macro_definition(input, macro_name, context)){
					success = false;				
				}
			} else {
				add_error(ERR_MACRO_NAME_MISSING, filename, context->line_number, NULL);
				success = false;
			}
		}

		context->line_number++;
		free(line);
	}

	fclose(input);

	/* If no macros were found, no need to create an output file
	if (!macros_present) {
		return !has_errors();
	}
 	*/

	/* Second pass: expand macros and write to the output file */
	input = fopen(filename, "r");
	if (!input) {
		add_error(ERR_FILE_NOT_FOUND, filename, 0, NULL);
		fclose(input);
		return false;
	}

	expand_macros(input, context);

	fclose(input);

	return success && !has_errors();
}

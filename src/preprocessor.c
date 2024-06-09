/* Handles macro expansion and prepares the source code for parsing.
** Reads the input file, processes macros, and writes the expanded code to a temporary file.
*/
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "error.h"

#define INITIAL_LINE_LENGTH 128

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

void expand_macros(FILE *input, FILE *output){
	char *line;
	char *token;
	Macro *macro;
	int i;

	while ((line = read_line(input)) != NULL){
		token = strtok(line, " \t\n");
		
		if (token != NULL){
			/*printf("%s\n", token);*/
			macro = find_macro(token);
			if (macro != NULL) {
				for (i = 0; i < macro->line_count; i++){
					fputs(macro->lines[i], output);
				}
			} else {
				fputs(line, output);
				fputs("\n", output);
			}
		} else {
			fputs("\n", output);
		}

		free(line);	
	}	
}


bool process_macro_definition(FILE *input, const char *name){
	Macro *new_macro;
	char *line;
	int i;
	new_macro = (Macro *)malloc(sizeof(Macro));
	strncpy(new_macro->name, name, sizeof(new_macro->name));
	new_macro->lines = NULL;
	new_macro->line_count = 0;
	new_macro->next = NULL;

	while ((line = read_line(input)) != NULL){
		
		printf("%s\n", line);
		if (strncmp(line, "endmacr", 7) == 0){
			free(line);
			break;		
		}
		new_macro->line_count++;
		new_macro->lines = (char **)realloc(new_macro->lines, new_macro->line_count * sizeof(char *));
		new_macro->lines[new_macro->line_count - 1] = line;
	}

	add_macro(new_macro);
	printf("name: %s\n", new_macro->name);
	for (i = 0; i <= new_macro->line_count; i++) {
		printf("line %d: %s\n", i, new_macro->lines[i]);
	}
	printf("line count: %d\n", new_macro->line_count);
	return true;
}

bool preprocess(const char *filename, const char *output_filename){
	FILE *input;
	FILE *output;
	char *line;
	char *token;
	char *macro_name;

	input = fopen(filename, "r");
	if (!input) {
		perror("Error opening input file");
		return false;
	}

	/* First pass: check for macros and process definitions */
	while ((line = read_line(input)) != NULL) {
		token = strtok(line, " \t\n");
		if (token != NULL && strcmp(token, "macr") == 0) {
			macro_name = strtok(NULL, " \t\n");
			if (macro_name != NULL) {
				if (!process_macro_definition(input, macro_name)) {
					fclose(input);
					free(line);
					return false;
				}
			} else {
				report_error("Macro name missing");
				fclose(input);
				free(line);
				return false;
			}
		}

		free(line);
	}

	fclose(input);

	/* If no macros were found, no need to create an output file */
	if (!macros_present) {
		return true;
	}

	/* Second pass: expand macros and write to the output file */
	input = fopen(filename, "r");
	if (!input) {
		perror("Error reopening input file");
		return false;
	}

	output = fopen(output_filename, "w");
	if (!output) {
		perror("Error opening output file");
		fclose(input);
		return false;
	}

	expand_macros(input, output);

	fclose(input);
	fclose(output);

	return true;
}

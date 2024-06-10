/* Coordinates the overall assembly process.
** Invokes preprocessing, parsing, code generation, and handles memory and label management integration.
** Responsible for initializing memory and handling the overall flow. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "error.h"
#include "assembler.h"

bool preprocess_all_files(int file_count, const char **filenames, Context *contexts) {
	int i;	
	bool success = true;
	for (i = 0; i < file_count; i++) {
		if (!preprocess(filenames[i], &contexts[i])) {
			success = false;
		}
	}
	return success;
}

void create_preprocessed_files(int file_count, Context *contexts) {
	int i, j;
	char output_filename[256];
	FILE *output;

	for (i = 0; i < file_count; i++) {
		sprintf(output_filename, "%s.preprocessed", contexts[i].filename);
		output = fopen(output_filename, "w");
		if (!output) {
			add_error(ERR_FILE_NOT_FOUND, output_filename, 0, NULL);
			continue;
		}

		for (j = 0; j < contexts[i].line_count; j++) {
			fputs(contexts[i].preprocessed_lines[j], output);
			fputs("\n", output);
			free(contexts[i].preprocessed_lines[j]);
		}
		fclose(output);
		printf("Preprocessing succeeded. Output written to %s\n", output_filename);
	}
}

bool assemble(const char *filename) {
	/* Additional assembly steps would go here (parsing, code generation, etc.) */

	return true;
}

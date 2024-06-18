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

bool assemble(const char *filename) {
	/* Additional assembly steps would go here (parsing, code generation, etc.) */

	return true;
}

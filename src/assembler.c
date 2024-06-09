/* Coordinates the overall assembly process.
** Invokes preprocessing, parsing, code generation, and handles memory and label management integration.
** Responsible for initializing memory and handling the overall flow. 
*/

#include "preprocessor.h"
#include "utils.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"


bool assemble(const char *filename) {
	char output_filename[256];
	sprintf(output_filename, "%s.preprocessed", filename);

	if (!preprocess(filename, output_filename)) {
		report_error("Preprocessing failed.");
		return false;
	}

	printf("Preprocessing succeeded. Output written to %s\n", output_filename);

	/* Additional assembly steps would go here (parsing, code generation, etc.) */

	return true;
}

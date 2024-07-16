/* Coordinates the overall assembly process.
** Invokes preprocessing, parsing, code generation, and handles memory and label management integration.
** Responsible for initializing memory and handling the overall flow. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "error.h"
#include "operations.h"
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

bool assemble(const char *filename, AssembledProgram *program) {
	char line[256];
	unsigned int address = 100; /* Starting address for instructions */
	unsigned short opcode;
	char *mnemonic;

	FILE *file = fopen(filename, "r");
	if (!file) {
		perror("Failed to open source file");
		return false;
	}

	while (fgets(line, sizeof(line), file)) {
		mnemonic = strtok(line, " \t\n");
		if (mnemonic) {
			opcode = get_opcode(mnemonic);
			if (opcode != 0xFFFF) {
				write_word(program, address++, opcode << 10); /* Store opcode in the first 6 bits */
			} else {
				add_error(ERR_INVALID_OPCODE, filename, address, mnemonic);
			}
		}
	}

	fclose(file);
	return !has_errors();
}

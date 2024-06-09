/* The entry point of the program.
** Handles command-line arguments and initiates the assembly process by calling the assemble function.
*/

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <sourcefile.asm>\n", argv[0]);
		return 1;
	}

	if (assemble(argv[1])) {
		printf("Assembly completed successfully.\n");
	} else {
		printf("Assembly failed.\n");
	}

	return 0;
}


/* handle output file generation */
#include "output.h"
#include <stdlib.h>
#include <string.h>

void init_program(AssembledProgram *program) {
	program->size = 0;
	memset(program->memory, 0, sizeof(program->memory));
}

void write_word(AssembledProgram *program, unsigned int address, unsigned short word) {
	if (address < MEMORY_SIZE) {
		program->memory[address].address = address;
		program->memory[address].word = word & 0x7FFF; /* Ensure it's 15-bit */
		if (address >= program->size) {
			program->size = address + 1;
		}
	}
}


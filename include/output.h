#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

#define MEMORY_SIZE 4096
#define WORD_SIZE 15

typedef struct {
	unsigned int address;
	unsigned short word; /* 15-bit word (we'll use 16 bits for convenience) */
} MemoryWord;

typedef struct {
	MemoryWord memory[MEMORY_SIZE];
	int size;
} AssembledProgram;

void init_program(AssembledProgram *program);
void write_word(AssembledProgram *program, unsigned int address, unsigned short word);

#endif


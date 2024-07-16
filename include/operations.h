#ifndef OPERATIONS_H
#define OPERATIONS_H

typedef struct {
	const char *mnemonic;
	unsigned short opcode;
} Operation;

#define NUM_OPERATIONS 16

const Operation* get_operations();
unsigned short get_opcode(const char *mnemonic);

#endif


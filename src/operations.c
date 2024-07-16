#include "operations.h"
#include <string.h>

static const Operation operations[NUM_OPERATIONS] = {
	{"mov", 0x00},
	{"cmp", 0x01},
	{"add", 0x02},
	{"sub", 0x03},
	{"not", 0x04},
	{"clr", 0x05},
	{"lea", 0x06},
	{"inc", 0x07},
	{"dec", 0x08},
	{"jmp", 0x09},
	{"bne", 0x0A},
	{"red", 0x0B},
	{"prn", 0x0C},
	{"jsr", 0x0D},
	{"rts", 0x0E},
	{"stop", 0x0F}
};

const Operation* get_operations() {
	return operations;
}

unsigned short get_opcode(const char *mnemonic) {
	int i;
	for (i = 0; i < NUM_OPERATIONS; i++) {
		if (strcmp(operations[i].mnemonic, mnemonic) == 0) {
			return operations[i].opcode;
		}
	}
	return 0xFFFF; /* Invalid opcode */
}


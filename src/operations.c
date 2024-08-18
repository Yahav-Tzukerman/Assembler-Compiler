#include <string.h>
#include "operations.h"

/**
 * @brief Array of supported operations and their corresponding opcodes.
 *
 * This array contains the supported assembly operations (mnemonics) and their associated
 * opcodes. The operations are used during the assembly process to translate mnemonics into
 * machine code.
 */
static const Operation operations[NUM_OPERATIONS] = {
        {"mov", 0x00},
        {"cmp", 0x01},
        {"add", 0x02},
        {"sub", 0x03},
        {"lea", 0x04},
        {"clr", 0x05},
        {"not", 0x06},
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

/**
 * @brief Retrieve the array of supported operations.
 *
 * This function returns a pointer to the array of supported operations, which can be used
 * to look up the opcode associated with a given mnemonic.
 *
 * @return A pointer to the array of supported operations.
 */
const Operation* get_operations() {
    return operations;
}

/**
 * @brief Get the opcode for a given mnemonic.
 *
 * This function searches for the specified mnemonic in the operations array and returns the
 * corresponding opcode. If the mnemonic is not found, it returns 0xFFFF to indicate an invalid opcode.
 *
 * @param mnemonic The mnemonic to look up.
 * @return The opcode associated with the mnemonic, or 0xFFFF if the mnemonic is invalid.
 */
unsigned short get_opcode(const char *mnemonic) {
    int i;
    for (i = 0; i < NUM_OPERATIONS; i++) {
        if (strcmp(operations[i].mnemonic, mnemonic) == 0) {
            return operations[i].opcode;
        }
    }
    return 0xFFFF; /* Invalid opcode */
}

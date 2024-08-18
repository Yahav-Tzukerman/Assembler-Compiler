#ifndef OPERATIONS_H
#define OPERATIONS_H

/**
 * @brief Structure to represent an assembly operation.
 *
 * This structure contains the mnemonic (name) of the operation and its corresponding opcode.
 */
typedef struct {
    const char *mnemonic;         /**< The mnemonic (name) of the operation. */
    unsigned short opcode;        /**< The opcode associated with the mnemonic. */
} Operation;

#define NUM_OPERATIONS 16          /**< Number of supported operations. */

/**
 * @brief Retrieve the array of supported operations.
 *
 * @return A pointer to the array of supported operations.
 */
const Operation* get_operations();

/**
 * @brief Get the opcode for a given mnemonic.
 *
 * @param mnemonic The mnemonic to look up.
 * @return The opcode associated with the mnemonic, or 0xFFFF if the mnemonic is invalid.
 */
unsigned short get_opcode(const char *mnemonic);

#endif /* OPERATIONS_H */

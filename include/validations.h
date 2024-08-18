#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include "utils.h"
#include "label.h"
#include "memory.h"

/**
 * @brief Validates the name of a macro.
 *
 * @param macroName The name of the macro to validate.
 * @return True if the macro name is valid, false otherwise.
 */
bool validate_macro_name(char* macroName);

/**
 * @brief Validates the name of a label.
 *
 * @param label_name The name of the label to validate.
 * @param memory Pointer to the Memory structure for context.
 * @return True if the label name is valid, false otherwise.
 */
bool validate_label_name(char *label_name, Memory *memory);

/**
 * @brief Validates a data value.
 *
 * @param data The data value to validate.
 * @return True if the data value is valid, false otherwise.
 */
bool validate_data(char *data);

/**
 * @brief Validates a string.
 *
 * @param string The string to validate.
 * @return True if the string is valid, false otherwise.
 */
bool validate_string(char *string);

/**
 * @brief Validates an operand.
 *
 * @param operand The operand to validate.
 * @param memory Pointer to the Memory structure for context.
 * @return True if the operand is valid, false otherwise.
 */
bool validate_operand(char *operand, Memory *memory);

/**
 * @brief Validates an instruction.
 *
 * @param operation The operation (mnemonic) of the instruction.
 * @param dest_mode The addressing mode of the destination operand.
 * @param source_mode The addressing mode of the source operand.
 * @param memory Pointer to the Memory structure for context.
 * @return True if the instruction is valid, false otherwise.
 */
bool validate_instruction(char *operation, int dest_mode, int source_mode, Memory *memory);

/**
 * @brief Validates a label declaration.
 *
 * @param label_name The name of the label to validate.
 * @param memory Pointer to the Memory structure for context.
 * @return True if the label declaration is valid, false otherwise.
 */
bool validate_label_declaration(char *label_name, Memory *memory);

#endif /* VALIDATIONS_H */

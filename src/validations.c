/**
 * @file validations.c
 * @brief Implements various validation functions used throughout the assembly process.
 *
 * This module provides functions to validate instructions, operands, labels, strings,
 * and macros, ensuring the source code adheres to the assembly language syntax.
 */

#include "validations.h"
#include "preprocessor.h"
#include "memory.h"
#include "error.h"
#include "constants.h"
#include <string.h>
#include <ctype.h>

/* Reserved words in the assembly language */
const char *reserved_words[] = {
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
        ".data", ".string", ".extern", ".intern", NULL
};

/* Array of valid command strings */
const char* validCommands[] = {
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
        "macr", "endmar"
};

/* Number of valid commands in the array */
const int validCommandsCount = sizeof(validCommands) / sizeof(validCommands[0]);

/**
 * @brief Checks if a label name is a reserved word.
 *
 * @param label_name The label name to check.
 * @return true if the label name is reserved, false otherwise.
 */
bool is_reserved_word(const char *label_name) {
    int i;
    for (i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(label_name, reserved_words[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Validates an instruction based on its operation and addressing modes.
 *
 * Ensures that the instruction uses valid addressing modes for both source and destination operands.
 *
 * @param operation The operation (mnemonic) of the instruction.
 * @param dest_mode The addressing mode of the destination operand.
 * @param source_mode The addressing mode of the source operand.
 * @param memory Pointer to the Memory structure, used for error reporting.
 * @return true if the instruction is valid, false otherwise.
 */
bool validate_instruction(char *operation, int dest_mode, int source_mode, Memory *memory) {
    bool success = true;

    if (strcmp(operation, "mov") == 0 || strcmp(operation, "cmp") == 0 ||
        strcmp(operation, "add") == 0 || strcmp(operation, "sub") == 0 ||
        strcmp(operation, "lea") == 0) {

        if (source_mode == UNDEFINED_MODE) {
            add_error(ERR_INVALID_SOURCE_OPERAND, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        if (dest_mode == UNDEFINED_MODE) {
            add_error(ERR_INVALID_DEST_OPERAND, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        if (strcmp(operation, "cmp") != 0 && dest_mode == IMMEDIATE_MODE) {
            add_error(ERR_INVALID_ADDRESS_MODE, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        if (strcmp(operation, "lea") == 0) {
            if (source_mode != DIRECT_MODE) {
                add_error(ERR_INVALID_ADDRESS_MODE, memory->current_file, memory->current_line_number, memory->current_line);
                success = false;
            }
            if (dest_mode == IMMEDIATE_MODE) {
                add_error(ERR_INVALID_ADDRESS_MODE, memory->current_file, memory->current_line_number, memory->current_line);
                success = false;
            }
        }
        return success;
    } else if (strcmp(operation, "clr") == 0 || strcmp(operation, "not") == 0 ||
               strcmp(operation, "inc") == 0 || strcmp(operation, "dec") == 0 ||
               strcmp(operation, "jmp") == 0 || strcmp(operation, "bne") == 0 ||
               strcmp(operation, "red") == 0 || strcmp(operation, "jsr") == 0 ||
               strcmp(operation, "prn") == 0) {

        if (source_mode != UNDEFINED_MODE) {
            add_error(ERR_INVALID_SOURCE_OPERAND, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        if (dest_mode == UNDEFINED_MODE) {
            add_error(ERR_INVALID_DEST_OPERAND, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        if (strcmp(operation, "prn") == 0) {
            return success;
        }
        if (dest_mode == IMMEDIATE_MODE &&
            (strcmp(operation, "clr") == 0 || strcmp(operation, "not") == 0 ||
             strcmp(operation, "inc") == 0 || strcmp(operation, "dec") == 0 ||
             strcmp(operation, "red") == 0)) {
            add_error(ERR_INVALID_ADDRESS_MODE, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        if ((strcmp(operation, "jmp") == 0 || strcmp(operation, "bne") == 0 ||
             strcmp(operation, "jsr") == 0) && (dest_mode == IMMEDIATE_MODE || dest_mode == DIRECT_MODE)) {
            add_error(ERR_INVALID_ADDRESS_MODE, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        return success;
    } else if (strcmp(operation, "rts") == 0 || strcmp(operation, "stop") == 0) {
        if (source_mode != UNDEFINED_MODE || dest_mode != UNDEFINED_MODE) {
            add_error(ERR_INVALID_SOURCE_OPERAND, memory->current_file, memory->current_line_number, memory->current_line);
            success = false;
        }
        return success;
    }
    return success;
}

/**
 * @brief Validates an operand.
 *
 * Checks if the operand is a valid register, pointer, immediate value, or label.
 *
 * @param operand The operand to validate.
 * @param memory Pointer to the Memory structure for context and error reporting.
 * @return true if the operand is valid, false otherwise.
 */
bool validate_operand(char *operand, Memory *memory) {
    if (operand == NULL) {
        return false;
    }

    /* Check if the operand is a register */
    if (strlen(operand) == 2 && operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7') {
        return true;
    }

    /* Check if the operand is a pointer to a register */
    if (strlen(operand) == 3 && operand[0] == '*' && operand[1] == 'r' && operand[2] >= '0' && operand[2] <= '7') {
        return true;
    }

    /* Check if the operand is an immediate value */
    if (operand[0] == '#') {
        return validate_data(operand);
    }

    /* Check if the operand is a valid label */
    if (validate_label_name(operand, memory)) {
        return true;
    }

    return false;
}

/**
 * @brief Validates a string literal.
 *
 * Ensures the string is enclosed in double quotes and contains valid characters.
 *
 * @param string The string to validate.
 * @return true if the string is valid, false otherwise.
 */
bool validate_string(char *string) {
    int i;
    int len = strlen(string);

    /* Check if it starts and ends with double quotes */
    if (len < 2 || string[0] != '"' || string[len - 1] != '"') {
        return false;
    }

    /* Check for any invalid characters inside the string */
    for (i = 1; i < len - 1; i++) {
        if (string[i] < 32 || string[i] > 126) { /* Check for non-ASCII or control characters */
            return false;
        }
    }

    return true;
}

/**
 * @brief Validates a data value.
 *
 * Ensures the data is a valid integer, optionally preceded by a sign.
 *
 * @param data The data to validate.
 * @return true if the data is valid, false otherwise.
 */
bool validate_data(char *data) {
    if (data[0] == '#') {
        data++;
    }
    if (*data == '+' || *data == '-') {
        data++; /* Skip the sign if present */
    }
    /* Check if the remaining part is a valid integer */
    while (*data) {
        if (!isdigit(*data)) {
            return false; /* If a non-digit character is found, return false */
        }
        data++;
    }

    return true;
}

/**
 * @brief Validates a label declaration.
 *
 * Ensures the label name is valid and not already declared in the current scope.
 *
 * @param label_name The name of the label to validate.
 * @param memory Pointer to the Memory structure for context and error reporting.
 * @return true if the label declaration is valid, false otherwise.
 */
bool validate_label_declaration(char *label_name, Memory *memory) {
    Label *label;
    if (label_name == NULL) {
        return false;
    }
    if (!validate_label_name(label_name, memory)) {
        return false;
    }
    label = find_label(memory->label_list, label_name);
    if (label != NULL && label->declared) {
        add_error(ERR_LABEL_ALREADY_DECLARED, memory->current_file, memory->current_line_number, label_name);
        return false;
    }
    return true;
}

/**
 * @brief Validates a label name.
 *
 * Ensures the label name is valid, not a reserved word, and not already used as a macro.
 *
 * @param label_name The label name to validate.
 * @param memory Pointer to the Memory structure for context and error reporting.
 * @return true if the label name is valid, false otherwise.
 */
bool validate_label_name(char *label_name, Memory *memory) {
    Macro *macro;

    /* Check if the label starts with a letter */
    if (!isalpha(label_name[0])) {
        add_error(ERR_INVALID_LABEL_NAME, memory->current_file, memory->current_line_number, label_name);
        return false;
    }

    /* Check if the label is a reserved word */
    if (is_reserved_word(label_name)) {
        add_error(ERR_RESERVED_WORD, memory->current_file, memory->current_line_number, label_name);
        return false;
    }

    /* Check if the label name is used as a macro */
    macro = find_macro(label_name);
    if (macro != NULL) {
        add_error(ERR_LABEL_NAME_USED_AS_MACRO, memory->current_file, memory->current_line_number, label_name);
        return false;
    }

    return true;
}

/**
 * @brief Validates a macro name.
 *
 * Ensures the macro name is valid, not a reserved command, and not already used as a register name.
 *
 * @param macroName The macro name to validate.
 * @return true if the macro name is valid, false otherwise.
 */
bool validate_macro_name(char* macroName) {
    int i;

    /* Check if the macroName is not null */
    if (macroName == NULL) {
        return false;
    }

    /* Check if the macroName is not a reserved command */
    for (i = 0; i < validCommandsCount; i++) {
        if (strcmp(validCommands[i], macroName) == 0) {
            return false;
        }
    }

    /* Check that the macroName starts with a letter */
    if (!isalpha(macroName[0])) {
        return false;
    }

    /* Check if the macroName is a register name */
    if (strlen(macroName) == 2 && macroName[0] == 'r' && macroName[1] >= '0' && macroName[1] <= '7') {
        return false;
    }

    return true;
}

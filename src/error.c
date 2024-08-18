/**
 * @file error.c
 * @brief Manages error reporting throughout the assembly process.
 *
 * This file provides functions for error handling, including initialization, error logging,
 * and printing errors. It maintains a centralized error reporting mechanism to ensure consistent
 * error messages across the program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define INITIAL_ERROR_CAPACITY 10

/** Array to hold the errors encountered during execution. */
static Error *errors = NULL;
/** The current number of errors recorded. */
static int error_count = 0;
/** The current capacity of the errors array. */
static int error_capacity = 0;
/** A flag indicating whether any errors have occurred. */
static bool error_flag = false;

/** Array of error messages corresponding to error codes. */
const char *error_messages[] = {
        "File not found: %s",
        "Macro name missing.",
        "Macro name is not valid: %s",
        "Memory allocation failed.",
        "Unexpected token: %s",
        "Invalid label name: %s",
        "Label name used as macro: %s",
        "Label name already declared: %s",
        "Reserved word: %s",
        "Invalid data: %s",
        "Invalid string: %s",
        "Invalid instruction: %s",
        "Invalid source operand at the instruction: %s",
        "Invalid destination operand at the instruction: %s",
        "Invalid address mode at the instruction: %s",
        "Label already declared: %s",
        "Label: %s is declared as an extern.", /* ERR_LABEL_DECLARED_AS_EXTERNAL */
        "Label: %s is not declared.", /* ERR_LABEL_NOT_DECLARED */
        "Label: %s is declared as an entry.", /* ERR_ENTRY_LABEL_EXTERNAL */
        "Unknown error."
};

/**
 * @brief Initializes the error handling system.
 *
 * Allocates memory for storing errors and sets up the initial error handling state.
 */
void init_error_handling() {
    if (errors != NULL) {
        free(errors);
    }
    error_count = 0;
    error_capacity = INITIAL_ERROR_CAPACITY;
    errors = (Error *)malloc(error_capacity * sizeof(Error));
    if (errors == NULL) {
        fprintf(stderr, "Failed to allocate memory for error handling.\n");
        exit(EXIT_FAILURE);
    }
    error_flag = false;
}

/**
 * @brief Adds an error to the error log.
 *
 * @param code The error code representing the type of error.
 * @param filename The name of the file where the error occurred.
 * @param line The line number where the error occurred.
 * @param detail Additional details about the error.
 */
void add_error(ErrorCode code, const char *filename, int line, const char *detail) {
    if (error_count >= error_capacity) {
        error_capacity *= 2;
        errors = (Error *)realloc(errors, error_capacity * sizeof(Error));
        if (errors == NULL) {
            fprintf(stderr, "Failed to reallocate memory for error handling.\n");
            exit(EXIT_FAILURE);
        }
    }

    errors[error_count].code = code;
    strncpy(errors[error_count].filename, filename, sizeof(errors[error_count].filename) - 1);
    errors[error_count].filename[sizeof(errors[error_count].filename) - 1] = '\0';
    errors[error_count].line = line;

    if (detail) {
        sprintf(errors[error_count].message, error_messages[code], detail);
    } else {
        strncpy(errors[error_count].message, error_messages[code], sizeof(errors[error_count].message) - 1);
        errors[error_count].message[sizeof(errors[error_count].message) - 1] = '\0';
    }
    error_count++;
    error_flag = true;
}

/**
 * @brief Prints all recorded errors to stderr.
 *
 * This function iterates through the recorded errors and prints each one
 * to the standard error stream.
 */
void print_errors() {
    int i;
    for (i = 0; i < error_count; i++) {
        fprintf(stderr, "Error in file %s at line %d: %s\n", errors[i].filename, errors[i].line, errors[i].message);
    }
}

/**
 * @brief Checks if any errors have been recorded.
 *
 * @return true if errors have been recorded, false otherwise.
 */
bool has_errors() {
    return error_flag;
}

/**
 * @brief Frees the memory allocated for error handling.
 *
 * This function cleans up the memory used to store errors and resets
 * the error handling state.
 */
void free_errors() {
    free(errors);
    errors = NULL;
    error_count = 0;
    error_capacity = 0;
    error_flag = false;
}

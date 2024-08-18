/**
 * @file error.h
 * @brief Declares functions and structures for error handling.
 *
 * This header file contains the declarations for error handling functions, structures, and
 * enumerations used to manage errors consistently throughout the assembly process.
 */

#ifndef ERROR_H
#define ERROR_H

#include "utils.h"

/**
 * @enum ErrorCode
 * @brief Defines various error codes used throughout the assembler.
 */
typedef enum {
    ERR_FILE_NOT_FOUND,
    ERR_MACRO_NAME_MISSING,
    ERR_MACRO_NAME_IS_NOT_VALID,
    ERR_MEMORY_ALLOCATION_FAILED,
    ERR_UNEXPECTED_TOKEN,
    ERR_INVALID_LABEL_NAME,
    ERR_LABEL_NAME_USED_AS_MACRO,
    ERR_LABEL_NAME_ALREADY_DECLARED,
    ERR_RESERVED_WORD,
    ERR_INVALID_DATA,
    ERR_INVALID_STRING,
    ERR_INVALID_INSTRUCTION,
    ERR_INVALID_SOURCE_OPERAND,
    ERR_INVALID_DEST_OPERAND,
    ERR_INVALID_ADDRESS_MODE,
    ERR_LABEL_ALREADY_DECLARED,
    ERR_LABEL_DECLARED_AS_EXTERNAL,
    ERR_LABEL_NOT_DECLARED,
    ERR_ENTRY_LABEL_EXTERNAL,
    ERR_UNKNOWN /* Represents an unknown error */
} ErrorCode;

/**
 * @struct Error
 * @brief Represents an error that occurred during assembly.
 *
 * @var Error::code
 * The code identifying the type of error.
 * @var Error::message
 * The formatted error message.
 * @var Error::filename
 * The name of the file where the error occurred.
 * @var Error::line
 * The line number where the error occurred.
 */
typedef struct {
    ErrorCode code;
    char message[256];
    char filename[256];
    int line;
} Error;

void init_error_handling();
void add_error(ErrorCode code, const char *filename, int line, const char *detail);
void print_errors();
bool has_errors();
void free_errors();

#endif /* ERROR_H */

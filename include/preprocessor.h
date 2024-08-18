#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "utils.h"
#include <stdio.h>

#define MAX_LINE_LENGTH 256

/**
 * @brief Structure representing a macro in the assembly code.
 *
 * This structure holds the name of the macro, the lines of code that it
 * represents, and a pointer to the next macro in the list.
 */
typedef struct Macro {
    char name[32];           /**< Name of the macro. */
    char **lines;            /**< Array of lines that the macro expands to. */
    int line_count;          /**< Number of lines in the macro. */
    struct Macro *next;      /**< Pointer to the next macro in the list. */
} Macro;

/**
 * @brief Structure representing the context during preprocessing.
 *
 * This structure stores the filename, the line number, and the preprocessed lines.
 * It also manages the capacity of the line buffer.
 */
typedef struct {
    const char *filename;    /**< Name of the file being processed. */
    int line_number;         /**< Current line number being processed. */
    char **preprocessed_lines; /**< Array of preprocessed lines. */
    int line_count;          /**< Number of preprocessed lines. */
    int line_capacity;       /**< Current capacity of the preprocessed lines array. */
} Context;

/**
 * @brief Preprocesses the input file by expanding macros.
 *
 * @param filename The name of the input file.
 * @param context Pointer to the Context structure to store preprocessed lines.
 * @return True if preprocessing was successful, false otherwise.
 */
bool preprocess(const char *filename, Context *context);

/**
 * @brief Adds a new macro to the macro list.
 *
 * @param new_macro Pointer to the macro to be added.
 */
void add_macro(Macro *new_macro);

/**
 * @brief Finds a macro by its name.
 *
 * @param name The name of the macro to find.
 * @return Pointer to the macro if found, or NULL if not found.
 */
Macro* find_macro(const char *name);

/**
 * @brief Processes a macro definition.
 *
 * @param input Pointer to the input file.
 * @param name The name of the macro being defined.
 * @param context Pointer to the Context structure for error reporting.
 * @return True if the macro was successfully processed, false otherwise.
 */
bool process_macro_definition(FILE *input, char *name, Context *context);

/**
 * @brief Expands macros in the source file.
 *
 * @param input Pointer to the input file.
 * @param context Pointer to the Context structure to store preprocessed lines.
 */
void expand_macros(FILE *input, Context *context);

/**
 * @brief Adds a line of preprocessed code to the context.
 *
 * @param context Pointer to the Context structure containing preprocessed lines.
 * @param line The line to add.
 */
void add_preprocessed_line(Context *context, const char *line);

/**
 * @brief Frees the memory used by the context.
 *
 * @param context Pointer to the Context structure to be freed.
 */
void free_context(Context *context);

/**
 * @brief Frees the memory used by the macro list.
 */
void free_macros();

#endif /* PREPROCESSOR_H */

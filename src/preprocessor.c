/**
 * @file preprocessor.c
 * @brief Handles macro expansion and prepares the source code for parsing.
 *
 * This module reads the input file, processes macros, and writes the expanded code
 * to a temporary file. It manages the lifecycle of macros and ensures that the code
 * is properly expanded before being passed on to the parser.
 */

#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "validations.h"
#include "error.h"

#define INITIAL_LINE_CAPACITY 100

static Macro *macro_list = NULL;
static bool macros_present = false;

/**
 * @brief Adds a new macro to the macro list.
 *
 * This function adds a macro to the head of the macro list. The list is used to store
 * all macros defined during the preprocessing stage.
 *
 * @param new_macro Pointer to the macro to be added.
 */
void add_macro(Macro *new_macro) {
    new_macro->next = macro_list;
    macro_list = new_macro;
    macros_present = true;
}

/**
 * @brief Finds a macro by its name.
 *
 * Searches the macro list for a macro with the specified name. If found, it returns
 * a pointer to the macro; otherwise, it returns NULL.
 *
 * @param name The name of the macro to find.
 * @return Pointer to the macro if found, or NULL if not found.
 */
Macro* find_macro(const char *name) {
    Macro *current = macro_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Adds a line of preprocessed code to the context.
 *
 * This function adds a line to the preprocessed lines in the context. If necessary,
 * it reallocates memory to accommodate additional lines.
 *
 * @param context Pointer to the Context structure containing preprocessed lines.
 * @param line The line to add.
 */
void add_preprocessed_line(Context *context, const char *line) {
    if (context->line_count >= context->line_capacity) {
        context->line_capacity *= 2;
        context->preprocessed_lines = (char **)realloc(context->preprocessed_lines, context->line_capacity * sizeof(char *));
        if (context->preprocessed_lines == NULL) {
            add_error(ERR_MEMORY_ALLOCATION_FAILED, context->filename, context->line_number, NULL);
            return;
        }
    }
    context->preprocessed_lines[context->line_count] = str_duplicate(line);
    context->line_count++;
}

/**
 * @brief Expands macros in the source file.
 *
 * This function processes the source file, expanding macros as it encounters them.
 * The expanded lines are added to the context's preprocessed lines.
 *
 * @param input Pointer to the input file.
 * @param context Pointer to the Context structure to store preprocessed lines.
 */
void expand_macros(FILE *input, Context *context) {
    char *line;
    char *line_duplicate;
    char *token;
    Macro *macro;
    int i;
    bool skip_lines = false;

    context->line_number = 1;
    while ((line = read_line(input)) != NULL) {
        line_duplicate = str_duplicate(line);
        token = strtok(line_duplicate, " \t\n");

        if (token != NULL) {
            if (strcmp(token, "macr") == 0) {
                skip_lines = true;
            } else if (strcmp(token, "endmacr") == 0) {
                skip_lines = false;
                free(line_duplicate);
                free(line);
                context->line_number++;
                continue;
            }

            if (skip_lines) {
                free(line_duplicate);
                free(line);
                context->line_number++;
                continue;
            }

            macro = find_macro(token);
            if (macro != NULL) {
                for (i = 0; i < macro->line_count; i++) {
                    add_preprocessed_line(context, macro->lines[i]);
                }
            } else {
                add_preprocessed_line(context, line);
            }
        } else {
            add_preprocessed_line(context, "");
        }
        free(line_duplicate);
        free(line);
        context->line_number++;
    }
}

/**
 * @brief Processes a macro definition.
 *
 * This function reads the lines of a macro definition from the input file and stores
 * them in a new Macro structure. It then adds the macro to the macro list.
 *
 * @param input Pointer to the input file.
 * @param name The name of the macro being defined.
 * @param context Pointer to the Context structure for error reporting.
 * @return True if the macro was successfully processed, false otherwise.
 */
bool process_macro_definition(FILE *input, char *name, Context *context) {
    int i;
    Macro *new_macro;
    char *line;
    char *trimmed_line;

    if (!validate_macro_name(name)) {
        add_error(ERR_MACRO_NAME_IS_NOT_VALID, context->filename, context->line_number, name);
        return false;
    }

    new_macro = (Macro *)malloc(sizeof(Macro));
    if (new_macro == NULL) {
        add_error(ERR_MEMORY_ALLOCATION_FAILED, context->filename, context->line_number, NULL);
        return false;
    }

    strncpy(new_macro->name, name, sizeof(new_macro->name));
    new_macro->lines = NULL;
    new_macro->line_count = 0;
    new_macro->next = NULL;

    while ((line = read_line(input)) != NULL) {
        trimmed_line = trim_whitespace(line);
        if (strncmp(trimmed_line, "endmacr", 7) == 0) {
            free(line);
            break;
        }

        new_macro->line_count++;
        new_macro->lines = (char **)realloc(new_macro->lines, new_macro->line_count * sizeof(char *));
        if (new_macro->lines == NULL) {
            add_error(ERR_MEMORY_ALLOCATION_FAILED, context->filename, context->line_number, NULL);
            free(line);
            for (i = 0; i < new_macro->line_count - 1; i++) {
                free(new_macro->lines[i]);
            }
            free(new_macro->lines);
            free(new_macro);
            return false;
        }
        new_macro->lines[new_macro->line_count - 1] = line;
        context->line_number++;
    }

    add_macro(new_macro);
    return true;
}

/**
 * @brief Frees the memory used by the context.
 *
 * This function frees the memory allocated for the preprocessed lines in the context.
 *
 * @param context Pointer to the Context structure to be freed.
 */
void free_context(Context *context) {
    int i;
    for (i = 0; i < context->line_count; i++) {
        free(context->preprocessed_lines[i]);
    }
    free(context->preprocessed_lines);
    context->preprocessed_lines = NULL;
}

/**
 * @brief Frees the memory used by the macro list.
 *
 * This function frees the memory allocated for all macros in the macro list.
 */
void free_macros() {
    int i;
    Macro *current = macro_list;
    Macro *next;

    while (current != NULL) {
        next = current->next;
        for (i = 0; i < current->line_count; i++) {
            free(current->lines[i]);
        }
        free(current->lines);
        free(current);
        current = next;
    }
    macro_list = NULL;
}

/**
 * @brief Preprocesses the input file by expanding macros.
 *
 * This function reads the input file, processes any macro definitions, and expands them.
 * The expanded lines are stored in the context structure.
 *
 * @param filename The name of the input file.
 * @param context Pointer to the Context structure to store preprocessed lines.
 * @return True if preprocessing was successful, false otherwise.
 */
bool preprocess(const char *filename, Context *context) {
    FILE *input;
    char *line;
    char *token;
    char *macro_name;
    bool success = true;

    context->filename = filename;
    context->line_number = 1;
    context->preprocessed_lines = (char **)malloc(INITIAL_LINE_CAPACITY * sizeof(char *));
    context->line_count = 0;
    context->line_capacity = INITIAL_LINE_CAPACITY;

    if (context->preprocessed_lines == NULL) {
        add_error(ERR_MEMORY_ALLOCATION_FAILED, filename, 0, NULL);
        return false;
    }

    input = fopen(filename, "r");
    if (!input) {
        add_error(ERR_FILE_NOT_FOUND, filename, 0, NULL);
        free(context->preprocessed_lines);
        return false;
    }

    /* First pass: check for macros and process definitions */
    while ((line = read_line(input)) != NULL) {
        token = strtok(line, " \t\n");
        if (token != NULL && strcmp(token, "macr") == 0) {
            macro_name = strtok(NULL, " \t\n");
            if (macro_name != NULL) {
                if (!process_macro_definition(input, macro_name, context)) {
                    success = false;
                }
            } else {
                add_error(ERR_MACRO_NAME_MISSING, filename, context->line_number, NULL);
                success = false;
            }
        }

        context->line_number++;
        free(line);
    }

    fclose(input);

    /* Expand macros and write to the output file */
    input = fopen(filename, "r");
    if (!input) {
        add_error(ERR_FILE_NOT_FOUND, filename, 0, NULL);
        return false;
    }

    expand_macros(input, context);

    fclose(input);

    return success && !has_errors();
}

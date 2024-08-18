/**
 * @file label.h
 * @brief Provides the interface for managing labels in the assembly process.
 *
 * This header defines the structure and functions for creating, adding, finding,
 * and freeing labels in the context of assembly language processing.
 */

#ifndef LABEL_H
#define LABEL_H

#include "utils.h"

/**
 * @brief Structure to represent a label in assembly code.
 */
typedef struct Label {
    char *name;               /**< The name of the label */
    char *file_name;          /**< The file name where the label is declared */
    int address;              /**< The memory address associated with the label */
    int line_number;          /**< The line number where the label is declared */
    bool is_instruction;      /**< Whether the label is associated with an instruction */
    bool entry;               /**< Whether the label is marked as an entry */
    bool external;            /**< Whether the label is marked as external */
    bool declared;            /**< Whether the label has been declared */
    struct Label *next;       /**< Pointer to the next label in the list */
} Label;

/**
 * @brief Creates a new label with the provided details.
 *
 * @param name The name of the label.
 * @param address The address associated with the label.
 * @param is_instruction Whether the label is associated with an instruction.
 * @param entry Whether the label is marked as an entry.
 * @param external Whether the label is marked as external.
 * @param file_name The name of the file where the label is declared.
 * @param declared Whether the label has been declared.
 * @param line_number The line number where the label is declared.
 * @return A pointer to the newly created label, or NULL if memory allocation fails.
 */
Label* create_label(char *name, int address, bool is_instruction, bool entry, bool external, char *file_name, bool declared, int line_number);

/**
 * @brief Adds a label to the label list.
 *
 * @param label_list Pointer to the head of the label list.
 * @param name The name of the label.
 * @param address The address associated with the label.
 * @param is_instruction Whether the label is associated with an instruction.
 * @param entry Whether the label is marked as an entry.
 * @param external Whether the label is marked as external.
 * @param file_name The name of the file where the label is declared.
 * @param declared Whether the label has been declared.
 * @param line_number The line number where the label is declared.
 * @return True if the label was added successfully, otherwise false.
 */
bool add_label(Label **label_list, char *line, int address, bool is_instruction, bool entry, bool external, char *file_name, bool declared, int line_number);

/**
 * @brief Finds a label by its name in the label list.
 *
 * @param label_list The list of labels.
 * @param name The name of the label to find.
 * @return A pointer to the found label, or NULL if the label is not found.
 */
Label* find_label(Label *label_list, char *name);

/**
 * @brief Checks if a token matches any label in the label list.
 *
 * @param token The token to check.
 * @param label_list The list of labels.
 * @return True if the token matches a label, otherwise false.
 */
bool is_label(const char *token, Label *label_list);

/**
 * @brief Frees the memory allocated for the label list.
 *
 * @param label_list The list of labels to free.
 */
void free_labels(Label *label_list);

#endif /* LABEL_H */

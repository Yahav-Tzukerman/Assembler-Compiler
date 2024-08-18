/**
 * @file label.c
 * @brief Manages labels and pending writes in the assembly process.
 *
 * This file provides functions to create, add, find, and free labels. It integrates
 * with the memory module to handle forward references in assembly code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"
#include "utils.h"

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
Label* create_label(char *name, int address, bool is_instruction, bool entry, bool external, char *file_name, bool declared, int line_number) {
    Label *new_label = (Label *)malloc(sizeof(Label));
    if (new_label == NULL) {
        fprintf(stderr, "Memory allocation error for label\n");
        return NULL;
    }

    new_label->name = str_duplicate(name);
    if (new_label->name == NULL) {
        fprintf(stderr, "Memory allocation error for label name\n");
        free(new_label);  /* Free the already allocated memory for the label */
        return NULL;
    }

    new_label->file_name = str_duplicate(file_name);
    if (new_label->file_name == NULL) {
        fprintf(stderr, "Memory allocation error for label file_name\n");
        free(new_label->name);  /* Free the already allocated memory for the label name */
        free(new_label);         /* Free the already allocated memory for the label itself */
        return NULL;
    }

    new_label->address = address;
    new_label->is_instruction = is_instruction;
    new_label->entry = entry;
    new_label->external = external;
    new_label->declared = declared;
    new_label->line_number = line_number;
    new_label->next = NULL;

    return new_label;
}


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
bool add_label(Label **label_list, char *name, int address, bool is_instruction, bool entry, bool external, char *file_name, bool declared, int line_number) {
    /* Check if the label already exists */
    Label *new_label;
    Label *existing_label = find_label(*label_list, name);
    if (existing_label != NULL) {
        /* Free the existing label's memory before replacing it */
        free(existing_label->name);
        free(existing_label->file_name);
        existing_label->name = str_duplicate(name);
        existing_label->file_name = str_duplicate(file_name);
        existing_label->address = address;
        existing_label->is_instruction = is_instruction;
        existing_label->entry = entry;
        existing_label->external = external;
        existing_label->declared = declared;
        existing_label->line_number = line_number;
        return true;
    }

    /* Otherwise, add the new label */
    new_label = create_label(name, address, is_instruction, entry, external, file_name, declared, line_number);
    if (new_label == NULL) {
        return false;
    }

    if (*label_list == NULL) {
        *label_list = new_label;
    } else {
        Label *current = *label_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_label;
    }

    return true;
}


/**
 * @brief Finds a label by its name in the label list.
 *
 * @param label_list The list of labels.
 * @param name The name of the label to find.
 * @return A pointer to the found label, or NULL if the label is not found.
 */
Label* find_label(Label *label_list, char *name) {
    while (label_list != NULL) {
        if (strcmp(label_list->name, name) == 0) {
            return label_list;
        }
        label_list = label_list->next;
    }
    return NULL;
}

/**
 * @brief Checks if a token matches any label in the label list.
 *
 * @param token The token to check.
 * @param label_list The list of labels.
 * @return True if the token matches a label, otherwise false.
 */
bool is_label(const char *token, Label *label_list) {
    while (label_list != NULL) {
        if (strcmp(label_list->name, token) == 0) {
            return true;
        }
        label_list = label_list->next;
    }
    return false;
}

/**
 * @brief Frees the memory allocated for the label list.
 *
 * @param label_list The list of labels to free.
 */
void free_labels(Label *label_list) {
    while (label_list != NULL) {
        Label *next = label_list->next;
        if (label_list->name != NULL) {
            free(label_list->name);
        }

        free(label_list);
        label_list = next;
    }
}

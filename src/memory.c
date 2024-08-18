/**
 * @file memory.c
 * @brief Manages memory operations for the assembly process.
 *
 * This file provides functions to initialize memory, write to memory, manage 
 * instruction and data counters, and handle memory cleanup.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "utils.h"

/**
 * @brief Initializes the memory structure, setting all memory cells to zero and resetting counters.
 *
 * @param mem Pointer to the Memory structure to initialize.
 */
void initialize_memory(Memory *mem) {
    int i;
    for (i = 0; i < MEMORY_SIZE; i++) {
        mem->memory[i] = 0;
    }
    mem->IC = 0;
    mem->DC = 100;
    mem->current_line_number = 0;
    mem->instructionList = NULL;
    mem->dataList = NULL;
    mem->label_list = NULL;
    mem->current_line = NULL;
    mem->current_file = NULL;
}

/**
 * @brief Writes a word to the specified memory address and updates the corresponding linked list.
 *
 * @param mem Pointer to the Memory structure.
 * @param address The memory address to write to.
 * @param word The word to write to memory.
 * @param isInstruction Flag indicating whether the word is an instruction (1) or data (0).
 * @param label_name The name of the label associated with the memory word, if any.
 */
void write_to_memory(Memory *mem, int address, Word word, int isInstruction, char *label_name) {
    ListNode **list;
    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error in write_to_memory\n");
        return;
    }

    newNode->data = word & 0x7FFF;  /* Mask to 15 bits */
    newNode->address = address;
    newNode->label_name = str_duplicate(label_name);
    newNode->next = NULL;

    list = isInstruction ? &(mem->instructionList) : &(mem->dataList);
    if (*list == NULL) {
        *list = newNode;
    } else {
        ListNode *current = *list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/**
 * @brief Increments the Instruction Counter (IC).
 *
 * @param mem Pointer to the Memory structure.
 */
void increment_IC(Memory *mem) {
    if (mem->IC < MEMORY_SIZE) {
        mem->IC++;
    } else {
        fprintf(stderr, "Instruction Counter overflow\n");
    }
}

/**
 * @brief Increments the Data Counter (DC).
 *
 * @param mem Pointer to the Memory structure.
 */
void increment_DC(Memory *mem) {
    if (mem->DC < MEMORY_SIZE) {
        mem->DC++;
    } else {
        fprintf(stderr, "Data Counter overflow\n");
    }
}

/**
 * @brief Clears all memory, including the instruction list, data list, and labels.
 *
 * @param mem Pointer to the Memory structure to clear.
 */
void clear_memory(Memory *mem) {
    ListNode *current = mem->instructionList;
    while (current != NULL) {
        ListNode *temp = current;
        if (temp->label_name != NULL) {
            free(temp->label_name);
            temp->label_name = NULL;
        }
        current = current->next;
        free(temp);
    }
    mem->instructionList = NULL;

    current = mem->dataList;
    while (current != NULL) {
        ListNode *temp = current;
        if (temp->label_name != NULL) {
            free(temp->label_name);
            temp->label_name = NULL;
        }
        current = current->next;
        free(temp);
    }
    mem->dataList = NULL;

    free_labels(mem->label_list);
    mem->label_list = NULL;

    if (mem->current_line != NULL) {
        free(mem->current_line);
        mem->current_line = NULL;
    }

    if (mem->current_file != NULL) {
        free(mem->current_file);
        mem->current_file = NULL;
    }

    mem->IC = 0;
    mem->DC = 0;
    mem->current_line_number = 0;
}

/**
 * @brief Converts a word to its binary representation.
 *
 * @param word The word to convert.
 * @return A string representing the binary value of the word.
 */
char* word_to_binary(Word word) {
    int i;
    static char binary[WORD_SIZE + 1];
    binary[WORD_SIZE] = '\0';
    for (i = WORD_SIZE - 1; i >= 0; i--) {
        binary[i] = (word & 1) ? '1' : '0';
        word >>= 1;
    }
    return binary;
}

/**
 * @brief Prints the contents of memory, including instructions, data, and labels.
 *
 * @param mem Pointer to the Memory structure.
 */
void print_memory(const Memory *mem) {
    ListNode *node;
    Label *label = mem->label_list;
    printf("Instructions:\n");
    for (node = mem->instructionList; node != NULL; node = node->next) {
        printf("Address %04d: %s %s\n", node->address, word_to_binary(node->data), node->label_name);
    }
    printf("Data:\n");
    for (node = mem->dataList; node != NULL; node = node->next) {
        printf("Address %04d: %s\n", node->address, word_to_binary(node->data));
    }

    printf("Labels:\n");
    for (label = mem->label_list; label != NULL; label = label->next) {
        printf("name: %s: address: %04d entry:%d external: %d instruction: %d declared: %d declared in file: %s\n",
               label->name, label->address, label->entry, label->external, label->is_instruction,
               label->declared, label->file_name ? label->file_name : "NULL");
    }
}

/**
 * @brief Stores the current line in memory, freeing any previously stored line.
 *
 * @param mem Pointer to the Memory structure.
 * @param line The line to store.
 */
void write_current_line(Memory *mem, char *line) {
    if (mem->current_line != NULL) {
        free(mem->current_line);
        mem->current_line = NULL;
    }
    mem->current_line = str_duplicate(line);
}

/**
 * @brief Moves to the next word in the current line, updating the current_line pointer.
 *
 * @param mem Pointer to the Memory structure.
 */
void move_to_next_word(Memory *mem) {
    int i;
    char *new_line = str_duplicate(mem->current_line);

    for (i = 0; i < strlen(new_line); i++) {
        if (new_line[i] == ':') {
            new_line += i + 1;
            break;
        }
    }

    while (*new_line == ' ' || *new_line == '\t') {
        new_line++;
    }

    if (mem->current_line != NULL) {
        free(mem->current_line);
    }

    mem->current_line = str_duplicate(new_line);
}

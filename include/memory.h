/**
 * @file memory.h
 * @brief Provides the interface for managing memory in the assembly process.
 *
 * This header defines the structures and functions for initializing memory, writing data,
 * managing instruction and data counters, and clearing memory in an assembly language processor.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "label.h"

#define MEMORY_SIZE 4096  /* Number of memory cells */
#define WORD_SIZE 15      /* Each memory cell is 15 bits */

/**
 * @brief Defines a 15-bit word.
 */
typedef unsigned short Word;  /* 15-bit word (use unsigned short and mask to 15 bits) */

/**
 * @brief Structure to represent a node in the instruction or data list.
 */
typedef struct ListNode {
    int address;              /**< The memory address */
    Word data;                /**< The data stored at the address */
    char *label_name;         /**< The label associated with this memory location, if any */
    struct ListNode *next;    /**< Pointer to the next node */
} ListNode;

/**
 * @brief Structure to represent the memory, including counters and lists.
 */
typedef struct Memory {
    Word memory[MEMORY_SIZE]; /**< The actual memory cells */
    int IC;                   /**< Instruction Counter */
    int DC;                   /**< Data Counter */
    int current_line_number;  /**< The current line number being processed */
    char *current_line;       /**< The current line being processed */
    char *current_file;       /**< The current file being processed */
    ListNode *instructionList;/**< Linked list of instructions */
    ListNode *dataList;       /**< Linked list of data */
    Label *label_list;        /**< Linked list of labels */
} Memory;

/**
 * @brief Initializes the memory structure, setting all memory cells to zero and resetting counters.
 *
 * @param mem Pointer to the Memory structure to initialize.
 */
void initialize_memory(Memory *mem);

/**
 * @brief Writes a word to the specified memory address and updates the corresponding linked list.
 *
 * @param mem Pointer to the Memory structure.
 * @param address The memory address to write to.
 * @param word The word to write to memory.
 * @param isInstruction Flag indicating whether the word is an instruction (1) or data (0).
 * @param label_name The name of the label associated with the memory word, if any.
 */
void write_to_memory(Memory *mem, int address, Word word, int isInstruction, char *label_name);

/**
 * @brief Stores the current line in memory, freeing any previously stored line.
 *
 * @param mem Pointer to the Memory structure.
 * @param line The line to store.
 */
void write_current_line(Memory *mem, char *line);

/**
 * @brief Increments the Instruction Counter (IC).
 *
 * @param mem Pointer to the Memory structure.
 */
void increment_IC(Memory *mem);

/**
 * @brief Increments the Data Counter (DC).
 *
 * @param mem Pointer to the Memory structure.
 */
void increment_DC(Memory *mem);

/**
 * @brief Clears all memory, including the instruction list, data list, and labels.
 *
 * @param mem Pointer to the Memory structure to clear.
 */
void clear_memory(Memory *mem);

/**
 * @brief Converts a word to its binary representation.
 *
 * @param word The word to convert.
 * @return A string representing the binary value of the word.
 */
char *word_to_binary(Word word);

/**
 * @brief Prints the contents of memory, including instructions, data, and labels.
 *
 * @param mem Pointer to the Memory structure.
 */
void print_memory(const Memory *mem);

/**
 * @brief Moves to the next word in the current line, updating the current_line pointer.
 *
 * @param mem Pointer to the Memory structure.
 */
void move_to_next_word(Memory *mem);

#endif /* MEMORY_H */

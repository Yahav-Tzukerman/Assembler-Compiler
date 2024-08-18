/**
 * @file parser.h
 * @brief Provides the interface for parsing assembly code.
 *
 * This header file defines the functions for parsing assembly code, managing
 * labels, and handling different types of instructions and directives.
 */

#ifndef PARSER_H
#define PARSER_H

#include "memory.h"

/**
 * @brief Parses the given assembly file and processes its contents.
 *
 * @param filename The name of the file to parse.
 * @param mem Pointer to the Memory structure.
 */
void parse_file(const char *filename, Memory *mem);

/**
 * @brief Parses a line of assembly code.
 *
 * @param line The line of assembly code to parse.
 * @param mem Pointer to the Memory structure.
 */
void parse_line(char *line, Memory *mem);

/**
 * @brief Parses an instruction with no operands.
 *
 * @param line The current line containing the instruction.
 * @param mem Pointer to the Memory structure.
 */
void handle_no_operand_instruction(char *line, Memory *mem);

/**
 * @brief Handles label declarations and stores them in the memory structure.
 *
 * @param current_token The current token being processed.
 * @param mem Pointer to the Memory structure.
 */
void handle_label(const char *current_token, Memory *mem);

/**
 * @brief Determines the addressing mode of an operand.
 *
 * @param operand The operand to analyze.
 * @return The addressing mode of the operand.
 */
int get_addressing_mode(char *operand);

/**
 * @brief Handles different types of operands, including labels and immediate values.
 *
 * @param operand The operand to process.
 * @param address_mode The addressing mode of the operand.
 * @param mem Pointer to the Memory structure.
 */
void handle_operand(char *operand, int address_mode, Memory *mem);

/**
 * @brief Resolves labels and updates memory with final addresses.
 *
 * @param filename The name of the file being processed.
 * @param mem Pointer to the Memory structure.
 */
void second_parse(const char *filename, Memory *mem);

/**
 * @brief Checks if the current line is an .entry directive.
 *
 * @param mem Pointer to the Memory structure.
 * @return true if the current line is an .entry directive, false otherwise.
 */
bool is_entry(Memory *mem);

/**
 * @brief Checks if the current line is an .extern directive.
 *
 * @param mem Pointer to the Memory structure.
 * @return true if the current line is an .extern directive, false otherwise.
 */
bool is_extern(Memory *mem);

/**
 * @brief Handles operands that involve both source and destination registers.
 *
 * @param operand1 The first operand (source).
 * @param operand2 The second operand (destination).
 * @param dest_mode The addressing mode of the destination operand.
 * @param source_mode The addressing mode of the source operand.
 * @param mem Pointer to the Memory structure.
 */
void handle_double_register_operands(char *operand1, char *operand2, int dest_mode, int source_mode, Memory *mem);

#endif /* PARSER_H */

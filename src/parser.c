/**
 * @file parser.c
 * @brief Contains functions for parsing assembly language instructions and directives.
 *
 * This file provides functions to parse different types of assembly language
 * directives and instructions, manage labels, and handle memory allocation for
 * parsed data.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "memory.h"
#include "utils.h"
#include "error.h"
#include "label.h"
#include "operations.h"
#include "validations.h"
#include "constants.h"

/**
 * @brief Converts an integer to a 15-bit binary word (2's complement for negatives).
 *
 * @param value The integer value to convert.
 * @return A 15-bit Word representing the integer.
 */
Word int_to_word(int value) {
    if (value < 0) {
        value = (1 << WORD_SIZE) + value;  /* 2's complement for negative values */
    }
    return (Word) (value & 0x7FFF);  /* Mask to 15 bits */
}

/**
 * @brief Parses a .data directive and stores its values in memory.
 *
 * @param current_token The current token being processed.
 * @param mem Pointer to the Memory structure.
 */
void handle_data_directive(const char *current_token, Memory *mem) {
    Word word;
    int value;
    char *token = strtok(mem->current_line, "\t ,");
    while (strcmp(token, current_token) != 0) {
        token = strtok(NULL, "\t ,");
    }
    token = strtok(NULL, "\t ,");
    while (token != NULL) {
        if (!validate_data(token)) {
            add_error(ERR_INVALID_DATA, mem->current_file, mem->current_line_number, token);
            token = strtok(NULL, "\t ,");
            continue;
        }

        value = atoi(token);
        word = int_to_word(value);
        write_to_memory(mem, mem->DC, word, false, NULL);
        increment_DC(mem);
        token = strtok(NULL, "\t ,");
    }
}

/**
 * @brief Parses a .string directive and stores its values in memory.
 *
 * @param current_token The current token being processed.
 * @param mem Pointer to the Memory structure.
 */
void handle_string_directive(const char *current_token, Memory *mem) {
    char *str;
    char *token = strtok(mem->current_line, "\t ,");
    while (strcmp(token, current_token) != 0) {
        token = strtok(NULL, "\t ,");
    }
    token = strtok(NULL, "\t ,");
    if(!validate_string(token)){
        add_error(ERR_INVALID_STRING, mem->current_file, mem->current_line_number, token);
        return;
    }
    str = strchr(token, '"');
    if (str) {
        str++;  /* Skip the opening quote */
        while (*str && *str != '"') {
            write_to_memory(mem, mem->DC, (Word) *str++, 0, NULL);
            increment_DC(mem);
        }
        write_to_memory(mem, mem->DC, 0, 0, NULL);  /* Null terminator */
        increment_DC(mem);
    }
}

/**
 * @brief Determines if the current line contains an instruction.
 *
 * @param mem Pointer to the Memory structure.
 * @return true if the current line is an instruction, false otherwise.
 */
bool is_instruction(Memory *mem) {
    char *current_line = str_duplicate(mem->current_line);
    char *token = strtok(current_line, "\t ,");
    while (token != NULL) {
        if (strcmp(token, ".string") == 0 || strcmp(token, ".data") == 0 || strcmp(token, ".extern") == 0 ||
            strcmp(token, ".entry") == 0) {
            free(current_line);
            return false;
        } else if (strcmp(token, "mov") == 0 || strcmp(token, "cmp") == 0 || strcmp(token, "add") == 0 ||
                   strcmp(token, "sub") == 0 || strcmp(token, "lea") == 0 || strcmp(token, "clr") == 0 ||
                   strcmp(token, "not") == 0 || strcmp(token, "inc") == 0 || strcmp(token, "dec") == 0 ||
                   strcmp(token, "jmp") == 0 || strcmp(token, "bne") == 0 || strcmp(token, "red") == 0 ||
                   strcmp(token, "prn") == 0 || strcmp(token, "jsr") == 0 || strcmp(token, "rts") == 0 ||
                   strcmp(token, "stop") == 0) {
            free(current_line);
            return true;
        }
        token = strtok(NULL, "\t ,:");
    }
    free(current_line);
    return false;
}

/**
 * @brief Parses an instruction with no operands.
 *
 * @param line The current line containing the instruction.
 * @param mem Pointer to the Memory structure.
 */
void handle_no_operand_instruction(char *line, Memory *mem) {
    Word instruction = 0;

    if (strcmp(line, "rts") == 0) {
        instruction = (14 << 11);
    } else if (strcmp(line, "stop") == 0) {
        instruction = (15 << 11);
    } else {
        fprintf(stderr, "Unknown instruction: %s\n", line);
    }
    instruction |= 0x4; /* Set ARE to 100 */
    write_to_memory(mem, mem->IC, instruction, 1, NULL);
    increment_IC(mem);
}

/**
 * @brief Handles label declarations and stores them in the memory structure.
 *
 * @param current_token The current token being processed.
 * @param mem Pointer to the Memory structure.
 */
void handle_label(const char *current_token, Memory *mem) {
    char *label_name;
    char *current_line = str_duplicate(mem->current_line);
    char *token = strtok(current_line, "\t ,");
    Label *label;
    bool instruction;
    int address;

    while (strcmp(token, current_token) != 0) {
        token = strtok(NULL, "\t ,");
    }
    if(is_entry(mem)){ /* Skip entry labels */
        free(current_line);
        return;
    }

    if(is_extern(mem)){ /* Skip extern labels */
        free(current_line);
        return;
    }
    label_name = str_duplicate(token);
    label_name[strlen(label_name) - 1] = '\0';  /* Remove the colon */
    if(!validate_label_declaration(label_name, mem)){
        free(current_line);
        return;
    }
    label = find_label(mem->label_list, label_name);
    instruction = is_instruction(mem);
    address = instruction ? mem->IC : mem->DC;
    if (label != NULL) {
        label->is_instruction = instruction;
        label->address = address;
        label->declared = true;
        label->file_name = mem->current_file;
        label->line_number = mem->current_line_number;
    } else {
        add_label(&mem->label_list, label_name, address, instruction, false, false, mem->current_file, true, mem->current_line_number);
    }

    free(label_name);
    free(current_line);
}

/**
 * @brief Checks if the current line is an .entry directive.
 *
 * @param mem Pointer to the Memory structure.
 * @return true if the current line is an .entry directive, false otherwise.
 */
bool is_entry(Memory *mem) {
    char *current_line = str_duplicate(mem->current_line);
    char *token = strtok(current_line, "\t ,");
    while (token != NULL) {
        if (strcmp(token, ".entry") == 0) {
            free(current_line);
            return true;
        }
        token = strtok(NULL, "\t ,:");
    }
    free(current_line);
    return false;
}

/**
 * @brief Checks if the current line is an .extern directive.
 *
 * @param mem Pointer to the Memory structure.
 * @return true if the current line is an .extern directive, false otherwise.
 */
bool is_extern(Memory *mem) {
    char *current_line = str_duplicate(mem->current_line);
    char *token = strtok(current_line, "\t ,");
    while (token != NULL) {
        if (strcmp(token, ".extern") == 0) {
            free(current_line);
            return true;
        }
        token = strtok(NULL, "\t ,:");
    }
    free(current_line);
    return false;
}

/**
 * @brief Parses a generic instruction with operands.
 *
 * @param current_token The current token being processed.
 * @param mem Pointer to the Memory structure.
 */
void handle_instruction(const char *current_token, Memory *mem) {
    Word instruction;
    bool is_valid = true;
    int opcode, source_mode = UNDEFINED_MODE, dest_mode = UNDEFINED_MODE;
    char *line = str_duplicate(mem->current_line);
    char *operation, *operand1, *operand2;

    char *token = strtok(line, "\t ,");
    while (strcmp(token, current_token) != 0) {
        token = strtok(NULL, "\t ,");
    }

    operation = str_duplicate(token);  /* Get the operation (opcode) */
    operand1 = strtok(NULL, ", \t"); /* Get the first operand */
    operand2 = strtok(NULL, ", \t"); /* Get the second operand if exists */

    opcode = get_opcode(operation);

    if (operand1) {
        is_valid = validate_operand(operand1, mem);
        source_mode = get_addressing_mode(operand1);
        if (operand2) {
            is_valid = (validate_operand(operand2, mem) && is_valid);
            dest_mode = get_addressing_mode(operand2);
        } else {
            dest_mode = source_mode;  /* If only one operand, treat it as destination */
            source_mode = UNDEFINED_MODE;  /* No source operand */
        }
    }

    if(is_valid == false){
        free(line);
        free(operation);
        return;
    }

    if(!validate_instruction(operation, dest_mode, source_mode, mem)){
        free(line);
        free(operation);
        return;
    }

    source_mode = (source_mode == UNDEFINED_MODE) ? 0 : source_mode;
    dest_mode = (dest_mode == UNDEFINED_MODE) ? 0 : dest_mode;

    /* Construct the instruction word */
    instruction = (opcode << 11) | (source_mode << 7) | (dest_mode << 3) | ARE_ABSOLUTE; /* ARE is 100 */

    /* Write the instruction to memory */
    write_to_memory(mem, mem->IC, instruction, true, NULL);
    increment_IC(mem);

    /* Handle any additional words for operands (e.g., direct addresses) */
    if (operand1 != NULL && operand2 != NULL) {
        if ((dest_mode == INDIRECT_REGISTER_MODE || dest_mode == DIRECT_REGISTER_MODE)
            && (source_mode == INDIRECT_REGISTER_MODE || source_mode == DIRECT_REGISTER_MODE)) {
            handle_double_register_operands(operand1, operand2, dest_mode, source_mode, mem);
        } else {
            handle_operand(operand1, source_mode, mem);
            handle_operand(operand2, dest_mode, mem);
        }
    } else if (operand1 != NULL) {
        handle_operand(operand1, dest_mode, mem);
    }

    free(operation);
    free(line);
}

/**
 * @brief Determines the addressing mode of an operand.
 *
 * @param operand The operand to analyze.
 * @return The addressing mode of the operand.
 */
int get_addressing_mode(char *operand) {
    if (operand[0] == '#') {
        return IMMEDIATE_MODE;  /* Immediate addressing */
    } else if (operand[0] == 'r' && operand[1] >= FIRST_REGISTER && operand[1] <= LAST_REGISTER) {
        return DIRECT_REGISTER_MODE;  /* Register direct addressing */
    } else if (operand[0] == '*' && operand[1] == 'r' && operand[2] >= FIRST_REGISTER && operand[2] <= LAST_REGISTER) {
        return INDIRECT_REGISTER_MODE;  /* Indirect register addressing */
    } else {
        return DIRECT_MODE;  /* Direct addressing */
    }
}

/**
 * @brief Handles operands that involve both source and destination registers.
 *
 * @param operand1 The first operand (source).
 * @param operand2 The second operand (destination).
 * @param dest_mode The addressing mode of the destination operand.
 * @param source_mode The addressing mode of the source operand.
 * @param mem Pointer to the Memory structure.
 */
void handle_double_register_operands(char *operand1, char *operand2, int dest_mode, int source_mode, Memory *mem) {
    Word additional_word = 0;

    if (source_mode == INDIRECT_REGISTER_MODE){
        additional_word |= (Word) (operand1[2] - '0') << 3; /* Extract source register number */
    } else {
        additional_word |= (Word) (operand1[1] - '0') << 3; /* Extract source register number */
    }

    if (dest_mode == INDIRECT_REGISTER_MODE) {
        additional_word |= (Word) (operand2[2] - '0') << 6; /* Extract destination register number */
    } else {
        additional_word |= (Word) (operand2[1] - '0') << 6; /* Extract destination register number */
    }
    additional_word |= ARE_ABSOLUTE; /* set ARE is 100 */
    write_to_memory(mem, mem->IC, additional_word, true, NULL);
    increment_IC(mem);
}

/**
 * @brief Handles different types of operands, including labels and immediate values.
 *
 * @param operand The operand to process.
 * @param address_mode The addressing mode of the operand.
 * @param mem Pointer to the Memory structure.
 */
void handle_operand(char *operand, int address_mode, Memory *mem) {
    Word additional_word = 0;
    Label *label = NULL;
    char *label_name = NULL;

    switch (address_mode) {
        case IMMEDIATE_MODE:  /* Immediate addressing */
            if(!validate_data(operand)){
                add_error(ERR_INVALID_DATA, mem->current_file, mem->current_line_number, operand);
                return;
            }
            if (operand[0] == '#') { /* Skip the '#' character */
                operand++;
            }
            additional_word = int_to_word(atoi(operand));
            additional_word <<= 3;
            additional_word |= (ARE_ABSOLUTE);  /* Set ARE to 100 */
            break;

        case DIRECT_MODE:  /* Direct addressing */
            /* Assume the operand is a label, and resolve its address */
            if (is_label(operand, mem->label_list)) {
                label = find_label(mem->label_list, operand);
                additional_word = label->address;
                label_name = str_duplicate(label->name);
                label->line_number = mem->current_line_number;
            } else {
                add_label(&mem->label_list, operand, 0, false, false, false, mem->current_file, false, mem->current_line_number);
            }
            additional_word <<= 3;
            if (label != NULL) {
                if (label->external) {
                    additional_word |= (ARE_EXTERNAL);  /* Set ARE to 001 if external */
                } else {
                    additional_word |= (ARE_RELOCATABLE);  /* Set ARE to 010 */
                }
            } else {
                label_name = str_duplicate(operand);
                additional_word |= (ARE_EXTERNAL);  /* Set ARE to 001 */
            }
            break;

        case INDIRECT_REGISTER_MODE:  /* Indirect register addressing */
            /* Assume operand is in the form of *rX where X is the register number */
            additional_word = (Word) (operand[2] - '0');  /* Extract register number */
            additional_word <<= 6;
            additional_word |= (ARE_ABSOLUTE); /* Set ARE to 100 */
            break;

        case DIRECT_REGISTER_MODE:  /* Direct register addressing */
            /* Operand is in the form of rX where X is the register number */
            additional_word = (Word) (operand[1] - '0');  /* Extract register number */
            additional_word <<= 6;
            additional_word |= (ARE_ABSOLUTE);  /* Set ARE to 100 */
            break;

        default:
            add_error(ERR_INVALID_ADDRESS_MODE, mem->current_file, mem->current_line_number, mem->current_line);
            return;
    }

    /* Write the additional word to memory */
    write_to_memory(mem, mem->IC, additional_word, true, label_name);
    increment_IC(mem);

    if (label_name != NULL) {
        free(label_name);
    }
}

/**
 * @brief Parses an .entry directive and updates the corresponding label.
 *
 * @param current_token The current token being processed.
 * @param mem Pointer to the Memory structure.
 */
void handle_entry(const char *current_token, Memory *mem) {
    Label *label = NULL;
    char *token = strtok(mem->current_line, "\t ,");
    while (strcmp(token, current_token) != 0) {
        token = strtok(NULL, "\t ,");
    }
    token = strtok(NULL, "\t ,");
    if(validate_label_name(token, mem) == false){
        return;
    }
    label = find_label(mem->label_list, token);
    if (label != NULL) {
        if(label->external || label->entry || (label->declared && strcmp(label->file_name, mem->current_file) != 0)){
            add_error(ERR_LABEL_ALREADY_DECLARED, mem->current_file, mem->current_line_number, token);
        }
        label->entry = true;
        label->file_name = mem->current_file;
        label->line_number = mem->current_line_number;
    } else {
        add_label(&mem->label_list, token, 0, false, true, false, mem->current_file, false, mem->current_line_number);
    }
}

/**
 * @brief Parses an .extern directive and updates the corresponding label.
 *
 * @param current_token The current token being processed.
 * @param mem Pointer to the Memory structure.
 */
void handle_extern(const char *current_token, Memory *mem){
    Label *label = NULL;
    char *token = strtok(mem->current_line, "\t ,");
    while (strcmp(token, current_token) != 0) {
        token = strtok(NULL, "\t ,");
    }
    token = strtok(NULL, "\t ,");
    if(validate_label_name(token, mem) == false){
        return;
    }
    label = find_label(mem->label_list, token);
    if (label != NULL) {
        if(label->declared || label->external || label->entry){
            add_error(ERR_LABEL_ALREADY_DECLARED, mem->current_file, mem->current_line_number, token);
        }
        label->external = true;
        label->file_name = mem->current_file;
        label->line_number = mem->current_line_number;
    } else {
        add_label(&mem->label_list, token, 0, false, false, true, mem->current_file, false, mem->current_line_number);
    }
}

/**
 * @brief Parses a line of assembly code.
 *
 * @param line The line of assembly code to parse.
 * @param mem Pointer to the Memory structure.
 */
void parse_line(char *line, Memory *mem) {
    char *line_copy = str_duplicate(line);
    char *token = strtok(line, " \t\n");
    char *current_token;

    write_current_line(mem, line_copy);

    /* TODO: Validate memory */

    while (token != NULL) {
        if (token[0] == ';') {
            break; /* Skip comments */
        } else if (token[0] == '\0') {
            break;
        } else if (strchr(token, ':') != NULL) {
            handle_label(token, mem);
            move_to_next_word(mem);
        } else if (strstr(token, ".data") != NULL) {
            handle_data_directive(token, mem);
            break;
        } else if (strstr(token, ".string") != NULL) {
            handle_string_directive(token, mem);
            break;
        } else if (strstr(token, ".entry")) {
            handle_entry(token, mem);
            break;
        } else if (strstr(token, ".extern")) {
            handle_extern(token, mem);
            break;
        } else if (strstr(token, "stop") != NULL || strstr(token, "rts") != NULL) {
            handle_no_operand_instruction(token, mem);
            break;
        } else if (is_instruction(mem)) {
            handle_instruction(token, mem);
            break;
        } else {
            add_error(ERR_UNEXPECTED_TOKEN, mem->current_file, mem->current_line_number, token);
            break;
        }

        /* Reset token & Move to the next one */
        current_token = token;
        token = strtok(line_copy, " \t\n");

        while (strcmp(token, current_token) != 0) {
            token = strtok(NULL, " \t\n");
        }
        token = strtok(NULL, " \t\n");
    }
    free(line_copy);
}

/**
 * @brief Parses the given assembly file and processes its contents.
 *
 * @param filename The name of the file to parse.
 * @param mem Pointer to the Memory structure.
 */
void parse_file(const char *filename, Memory *mem) {
    FILE *input;
    char *line;
    mem->current_line_number = 0;
    mem->current_file = str_duplicate(filename);
    input = fopen(filename, "r");
    if (!input) {
        add_error(ERR_FILE_NOT_FOUND, filename, 0, NULL);
        return;
    }

    while ((line = read_line(input)) != NULL) {
        mem->current_line_number++;
        if(strcmp(line, "") != 0){
            parse_line(line, mem);
        }
        free(line);
    }

    fclose(input);
}

/**
 * @brief Resolves labels and updates memory with final addresses.
 *
 * @param filename The name of the file being processed.
 * @param mem Pointer to the Memory structure.
 */
void second_parse(const char *filename, Memory *mem) {
    Label *label;
    ListNode *node;
    Word word = 0;

    for (node = mem->instructionList; node != NULL; node = node->next) {
        if (node->label_name != NULL) {
            word = 0;
            if (is_label(node->label_name, mem->label_list)) {
                label = find_label(mem->label_list, node->label_name);
                if (label->external) {
                    word |= ARE_EXTERNAL; /* Set ARE to 001 */
                } else if(label->entry){
                    word |= ARE_RELOCATABLE; /* Set ARE to 010 */
                } else {
                    word |= ARE_ABSOLUTE; /* Set ARE to 100 */
                }
                word |= int_to_word(label->address) << 3;
                node->data = word;
            } else {
                word |= ARE_EXTERNAL; /* Set ARE to 001 */
                node->data = word;
            }
        }
    }
    for (label = mem->label_list; label != NULL; label = label->next) {
        if (label->external){
            if(!label->declared && strcmp(label->file_name, filename) == 0){
                add_error(ERR_LABEL_NOT_DECLARED, label->file_name, label->line_number, label->name);
            }
            if (label->entry) {
                add_error(ERR_LABEL_DECLARED_AS_EXTERNAL, label->file_name, label->line_number, label->name);
            }
        } else if (label->entry) {
            if(label->external){
                add_error(ERR_ENTRY_LABEL_EXTERNAL, label->file_name, label->line_number, label->name);
            }
            if (!label->declared && strcmp(label->file_name, filename) == 0) {
                add_error(ERR_LABEL_NOT_DECLARED, label->file_name, label->line_number, label->name);
            }
        } else if (!label->declared && strcmp(label->file_name, filename) == 0) {
            add_error(ERR_LABEL_NOT_DECLARED, label->file_name, label->line_number, label->name);
        }
    }
}

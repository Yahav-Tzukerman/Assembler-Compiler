/* Coordinates the overall assembly process.
** Invokes preprocessing, parsing, code generation, and handles memory and label management integration.
** Responsible for initializing memory and handling the overall flow. 
*/

#include <string.h>
#include "utils.h"
#include "error.h"
#include "assembler.h"
#include "memory.h"
#include "parser.h"
#include "file_manager.h"

/**
 * @brief Preprocesses all source files before assembly.
 *
 * This function iterates through each source file, invoking the preprocessing step
 * for each file. The results are stored in the provided contexts.
 *
 * @param file_count The number of files to preprocess.
 * @param filenames The array of file names to preprocess.
 * @param contexts The array of contexts to store preprocessing results.
 * @return true if all files were successfully preprocessed, false otherwise.
 */
bool preprocess_all_files(int file_count, const char **filenames, Context *contexts) {
    int i;
    bool success = true;
    for (i = 0; i < file_count; i++) {
        if (!preprocess(filenames[i], &contexts[i])) {
            success = false;
        }
    }
    return success;
}

/**
 * @brief Performs the assembly process on the given files.
 *
 * This function orchestrates the entire assembly process, including parsing, label handling,
 * and memory management. It processes each file in two passes and handles the output of the
 * assembled files.
 *
 * @param file_count The number of files to assemble.
 * @param filenames The array of file names to assemble.
 * @return true if assembly was successful for all files, false otherwise.
 */
bool assemble(int file_count, const char **filenames) {
    ListNode *node;
    Label *label;
    int i;
    bool success = true;
    Memory mem;

    initialize_memory(&mem);

    /* First parse */
    for (i = 0; i < file_count; i++) {
        parse_file(filenames[i], &mem);
    }

    /* Adjust label and node addresses */
    for (label = mem.label_list; label != NULL; label = label->next) {
        if (!label->is_instruction) {
            label->address = (label->address == 0) ? 0 : (label->address + mem.IC);
        } else {
            label->address += 100;
        }
    }

    for (node = mem.instructionList; node != NULL; node = node->next) {
        node->address += 100;
    }
    for (node = mem.dataList; node != NULL; node = node->next) {
        node->address += mem.IC;
    }

    /* Second parse */
    for (i = 0; i < file_count; i++) {
        second_parse(filenames[i], &mem);
    }

    /* Check for errors and write output files */
    if(has_errors()) {
        success = false;
    } else {
        write_output_files(filenames, file_count, &mem);
    }

    /* Clear memory */
    clear_memory(&mem);
    return success;
}

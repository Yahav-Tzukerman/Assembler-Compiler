/**
 * @file main.c
 * @brief The entry point of the assembler program.
 *
 * This file handles the command-line arguments, manages the flow of the assembly process,
 * and handles the creation of output files (.ob, .ext, .ent). It also manages error handling
 * and memory cleanup.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "preprocessor.h"
#include "error.h"
#include "file_manager.h"

/**
 * @brief The main function of the assembler program.
 *
 * This function is the entry point of the program. It processes command-line arguments,
 * initiates preprocessing, assembly, and handles the generation of output files.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return Returns 0 if the assembly process is successful, or 1 if it fails.
 */
int main(int argc, char *argv[]) {
    const char **filenames;
    int i, file_count;
    bool success;
    Context *contexts;

    /* Check if at least one source file is provided */
    if (argc < 2) {
        printf("Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }

    /* Initialize error handling */
    init_error_handling();

    /* Prepare filenames for processing */
    if (!prepare_filenames(argc, argv, &filenames, &file_count)) {
        print_errors();
        free_errors();
        return 1;
    }

    /* Allocate memory for contexts */
    contexts = (Context *)malloc(file_count * sizeof(Context));
    if (contexts == NULL) {
        fprintf(stderr, "Failed to allocate memory for contexts.\n");
        free_filenames(filenames, file_count);
        return 1;
    }

    /* Preprocess all files */
    success = preprocess_all_files(file_count, filenames, contexts);

    if (!success) {
        print_errors();
        printf("Assembly failed due to errors.\n");
    } else {
        /* Delete previous output files if they exist */
        delete_output_files(filenames, file_count);

        /* Create preprocessed files from the contexts */
        create_preprocessed_files(file_count, contexts);

        /* Fix the filenames after preprocessing */
        fix_filenames(filenames, file_count);

        /* Perform the assembly process */
        success = assemble(file_count, filenames);

        if (has_errors()) {
            print_errors();
            printf("Assembly failed due to errors.\n");
        } else {
            printf("Assembly completed successfully for all files.\n");
        }
    }

    /* Free allocated memory for contexts and filenames */
    for (i = 0; i < file_count; i++) {
        free_context(&contexts[i]);
    }
    free(contexts);
    free_filenames(filenames, file_count);

    /* Free resources used for macro processing and error handling */
    free_macros();
    free_errors();

    return success ? 0 : 1;
}

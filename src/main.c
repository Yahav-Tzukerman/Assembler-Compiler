/* The entry point of the program.
** Handles command-line arguments and initiates the assembly process by calling the assemble function.
*/

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
#include "preprocessor.h"
#include "error.h"
#include "file_manager.h"

int main(int argc, char *argv[]) {
	const char **filenames;
	int i, j, file_count;
	bool success;
	Context *contexts;


	if (argc < 2) {
		printf("Usage: %s <sourcefile>\n", argv[0]);
		return 1;
	}

	init_error_handling();

	if (!prepare_filenames(argc, argv, &filenames, &file_count)) {
		print_errors();
		free_errors();
		return 1;
	}

	contexts = (Context *)malloc(file_count * sizeof(Context));
	if (contexts == NULL) {
		fprintf(stderr, "Failed to allocate memory for contexts.\n");
		free_filenames(filenames, file_count);
		return 1;
	}

	success = preprocess_all_files(file_count, filenames, contexts);

	if (!success) {
		print_errors();
		printf("Assembly failed due to errors.\n");
	} else {
		create_preprocessed_files(file_count, contexts);
		printf("Assembly completed successfully for all files.\n");
	}

	free(contexts);
	free_filenames(filenames, file_count);
	free_errors();
	return success ? 0 : 1;
}


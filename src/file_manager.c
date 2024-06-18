#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "error.h"

#define MAX_FILENAME_LENGTH 256

void add_as_suffix(char *filename) {
	if (!strstr(filename, ".as")) {
		strcat(filename, ".as");
	}
}

bool prepare_filenames(int argc, char *argv[], const char ***filenames_ptr, int *file_count) {
	int i, j;
	char *filename_with_suffix;
	FILE *file;

	*file_count = argc - 1;
	*filenames_ptr = (const char **)malloc(*file_count * sizeof(char *));
	if (*filenames_ptr == NULL) {
		fprintf(stderr, "Failed to allocate memory for filenames.\n");
		return false;
	}

	for (i = 1; i < argc; i++) {
		filename_with_suffix = (char *)malloc(MAX_FILENAME_LENGTH * sizeof(char));
		if (filename_with_suffix == NULL) {
			fprintf(stderr, "Failed to allocate memory for filename.\n");
			for (j = 0; j < i ; j++) {
				free((void *)(*filenames_ptr)[j]);
			}
			free(*filenames_ptr);
			*filenames_ptr = NULL;
			return false;
		}
		strncpy(filename_with_suffix, argv[i], MAX_FILENAME_LENGTH - 1);
		filename_with_suffix[MAX_FILENAME_LENGTH - 1] = '\0';
		add_as_suffix(filename_with_suffix);

		file = fopen(filename_with_suffix, "r");
		if (!file) {
			add_error(ERR_FILE_NOT_FOUND, filename_with_suffix, 0, NULL);
			free(filename_with_suffix);
			for (j = 0; j < i - 1; j++) {
				free((void *)(*filenames_ptr)[j]);
			}
			free(*filenames_ptr);
			*filenames_ptr = NULL;
			return false;
		}
		fclose(file);
		(*filenames_ptr)[i - 1] = filename_with_suffix;
	}
	
	return true;
}

void free_filenames(const char **filenames, int file_count) {
	int i;
	if (filenames != NULL) {
		for (i = 0; i < file_count; i++) {
			if (filenames[i] != NULL) {
				free((void *)filenames[i]);
			}
		}
		free(filenames);
	}
}

void create_preprocessed_files(int file_count, Context *contexts) {
	int i, j;
	char output_filename[MAX_FILENAME_LENGTH];
	FILE *output;

	for (i = 0; i < file_count; i++) {

		size_t len = strlen(contexts[i].filename);
		if (len > 3 && strcmp(contexts[i].filename + len - 3, ".as") == 0) {
			strncpy(output_filename, contexts[i].filename, len - 3);
			output_filename[len - 3] = '\0';
			strcat(output_filename, ".am");
		} else {
			strncpy(output_filename, contexts[i].filename, len);
			output_filename[len] = '\0';
			strcat(output_filename, ".am");
		}

		output = fopen(output_filename, "w");
		if (!output) {
			add_error(ERR_FILE_NOT_FOUND, output_filename, 0, NULL);
			continue;
		}

		for (j = 0; j < contexts[i].line_count; j++) {
			fputs(contexts[i].preprocessed_lines[j], output);
			fputs("\n", output);
			free(contexts[i].preprocessed_lines[j]);
		}
		fclose(output);
		printf("Preprocessing succeeded. Output written to %s\n", output_filename);
	}
}


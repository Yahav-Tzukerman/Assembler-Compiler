#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "preprocessor.h"
#include "output.h"
#include "utils.h"

#define MAX_FILENAME_LENGTH 256

void add_as_suffix(char *filename);
void create_preprocessed_files(int file_count, Context *contexts);
bool prepare_filenames(int argc, char *argv[], const char ***filenames_ptr, int *file_count);
void free_filenames(const char **filenames, int file_count);
void save_program_to_file(const AssembledProgram *program, const char *filename);

#endif

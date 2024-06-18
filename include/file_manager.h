#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "preprocessor.h"
#include "utils.h"

void create_preprocessed_files(int file_count, Context *contexts);
bool prepare_filenames(int argc, char *argv[], const char ***filenames_ptr, int *file_count);
void free_filenames(const char **filenames, int file_count);

#endif

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "utils.h"
#include "preprocessor.h"

bool assemble(const char *filename);
bool preprocess_all_files(int file_count, const char **filenames, Context *contexts);
void create_preprocessed_files(int file_count, Context *contexts);

#endif /* ASSEMBLER_H */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "utils.h"
#include "output.h"
#include "preprocessor.h"

bool assemble(const char *filename, AssembledProgram *program);
bool preprocess_all_files(int file_count, const char **filenames, Context *contexts);

#endif /* ASSEMBLER_H */

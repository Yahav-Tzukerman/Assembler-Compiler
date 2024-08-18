#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "utils.h"
#include "preprocessor.h"

/**
 * @brief Performs the assembly process on the given files.
 *
 * @param file_count The number of files to assemble.
 * @param filenames The array of file names to assemble.
 * @return true if assembly was successful for all files, false otherwise.
 */
bool assemble(int file_count, const char **filenames);

/**
 * @brief Preprocesses all source files before assembly.
 *
 * @param file_count The number of files to preprocess.
 * @param filenames The array of file names to preprocess.
 * @param contexts The array of contexts to store preprocessing results.
 * @return true if all files were successfully preprocessed, false otherwise.
 */
bool preprocess_all_files(int file_count, const char **filenames, Context *contexts);

#endif /* ASSEMBLER_H */

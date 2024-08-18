/**
 * @file file_manager.h
 * @brief Declares functions for managing file operations within the assembler,
 * including preprocessing, output file generation, and file cleanup.
 */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "preprocessor.h"
#include "utils.h"
#include "memory.h"

/**
 * @brief Creates preprocessed files (.am) from the contexts provided.
 *
 * @param file_count The number of files to process.
 * @param contexts An array of Context structures containing preprocessed lines.
 */
void create_preprocessed_files(int file_count, Context *contexts);

/**
 * @brief Prepares and validates filenames based on the command-line arguments.
 *
 * This function appends the ".as" suffix if it's not already present and checks
 * that each file exists.
 *
 * @param argc The number of command-line arguments.
 * @param argv The list of command-line arguments.
 * @param filenames_ptr A pointer to store the list of prepared filenames.
 * @param file_count A pointer to store the count of filenames.
 * @return True if filenames were prepared successfully, false otherwise.
 */
bool prepare_filenames(int argc, char *argv[], const char ***filenames_ptr, int *file_count);

/**
 * @brief Frees the memory allocated for the list of filenames.
 *
 * @param filenames The list of filenames to free.
 * @param file_count The number of filenames in the list.
 */
void free_filenames(const char **filenames, int file_count);

/**
 * @brief Writes all necessary output files (.ent, .ext, .ob) based on the assembler's memory content.
 *
 * @param filenames The list of source filenames.
 * @param file_count The number of source files.
 * @param mem A pointer to the Memory structure containing the assembler's state.
 */
void write_output_files(const char **filenames, int file_count, Memory *mem);

/**
 * @brief Writes to the entry (.ent) file for a given label.
 *
 * @param name The label name.
 * @param address The label's address in memory.
 * @param filename The base filename for the output file.
 */
void write_to_entry_file(char *name, int address, char *filename);

/**
 * @brief Writes to the extern (.ext) file for a given label.
 *
 * @param name The label name.
 * @param address The label's address in memory.
 * @param filename The base filename for the output file.
 */
void write_to_extern_file(char *name, int address, char *filename);

/**
 * @brief Writes to the object (.ob) file for the assembled program.
 *
 * @param address The memory address to write to.
 * @param data The data to write.
 * @param filename The base filename for the output file.
 * @param mem A pointer to the Memory structure containing the assembler's state.
 */
void write_to_object_file(int address, int data, char *filename, Memory *mem);

/**
 * @brief Deletes a file with the specified filename and extension.
 *
 * @param filename The base filename.
 * @param extension The file extension to delete.
 */
void delete_file(const char *filename, const char *extension);

/**
 * @brief Extracts and formats filenames into a single string with underscores separating each filename.
 *
 * This function processes multiple filenames, removes extensions, replaces certain characters with underscores,
 * and combines them into a single string.
 *
 * @param filenames The list of filenames to format.
 * @param file_count The number of filenames in the list.
 * @return A newly allocated string containing the formatted filename.
 */
char* extract_and_format_filename(const char **filenames, int file_count);

/**
 * @brief Deletes all output files associated with the given filenames.
 *
 * This function deletes the `.ent`, `.ext`, `.ob`, and `.am` files generated during the assembly process.
 *
 * @param filenames The list of source filenames.
 * @param file_count The number of files in the list.
 */
void delete_output_files(const char **filenames, int file_count);

/**
 * @brief Replaces the file extension of each filename in the list with ".am".
 *
 * @param filenames The list of filenames to modify.
 * @param count The number of filenames in the list.
 */
void fix_filenames(const char **filenames, int count);

#endif /* FILE_MANAGER_H */

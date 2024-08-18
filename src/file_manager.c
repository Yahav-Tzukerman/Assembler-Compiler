/**
 * @file file_manager.c
 * @brief Handles file management tasks for the assembler, including preprocessing, 
 * writing output files, and cleaning up generated files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_manager.h"
#include "error.h"

#define MAX_FILENAME_LENGTH 256

/**
 * @brief Deletes all output files associated with the given filenames.
 *
 * This function deletes the `.ent`, `.ext`, `.ob`, and `.am` files generated during the assembly process.
 *
 * @param filenames The list of source filenames.
 * @param file_count The number of files in the list.
 */
void delete_output_files(const char **filenames, int file_count) {
    int i;
    char *formatted_filename = extract_and_format_filename(filenames, file_count);

    for (i = 0; i < file_count; i++) {
        delete_file(formatted_filename, ".ent");
        delete_file(formatted_filename, ".ext");
        delete_file(formatted_filename, ".ob");
        delete_file(filenames[i], ".am");
    }

    free(formatted_filename);
}

/**
 * @brief Deletes a file with the specified filename and extension.
 *
 * @param filename The base filename.
 * @param extension The file extension to delete.
 */
void delete_file(const char *filename, const char *extension) {
    char filepath[MAX_FILENAME_LENGTH + 3];  /* Original length + "./" + null terminator */
    FILE *file;
    int deleted;

    sprintf(filepath, "./%s%s", filename, extension);

    file = fopen(filepath, "r");
    if(file != NULL) {
        fclose(file);
        deleted = remove(filepath);
        if (deleted != 0) {
            add_error(ERR_FILE_NOT_FOUND, filepath, 0, NULL);
        }
    }
}

/**
 * @brief Extracts and formats filenames by combining them into a single string with underscores separating each filename.
 *
 * This function processes multiple filenames, removes extensions, replaces certain characters with underscores, 
 * and combines them into a single string.
 *
 * @param filenames The list of filenames to format.
 * @param file_count The number of filenames.
 * @return A newly allocated string containing the formatted filename.
 */
char* extract_and_format_filename(const char **filenames, int file_count) {
    int i;
    char *dot;
    size_t total_length = 0;
    char *final_filename, *iter, *last_slash, *p;

    /* Calculate the total length required for the final string */
    for (i = 0; i < file_count; i++) {
        const char *filename = filenames[i];
        last_slash = strrchr(filename, '/');
        p = (last_slash != NULL) ? last_slash + 1 : (char *)filename;
        dot = strrchr(p, '.');
        if (dot != NULL) {
            total_length += dot - p;
        } else {
            total_length += strlen(p);
        }
        total_length += 1; /* For underscores between filenames */
    }

    final_filename = (char *)malloc(total_length + 1);
    if (!final_filename) {
        perror("Memory allocation error");
        return NULL;
    }

    final_filename[0] = '\0';  /* Initialize the final string */

    for (i = 0; i < file_count; i++) {
        const char *filename = filenames[i];
        char *filename_copy = str_duplicate(filename);
        last_slash = strrchr(filename_copy, '/');
        p = (last_slash != NULL) ? last_slash + 1 : filename_copy;

        dot = strrchr(p, '.');
        if (dot != NULL) {
            *dot = '\0';  /* Remove the extension */
        }

        for (iter = p; *iter != '\0'; iter++) {
            if (*iter == ' ' || *iter == '/' || *iter == '\\' || *iter == '.') {
                *iter = '_';
            }
        }

        strcat(final_filename, p);
        if (i < file_count - 1) {
            strcat(final_filename, "_");  /* Add underscore between parts */
        }
        free(filename_copy);
    }

    return final_filename;
}

/**
 * @brief Writes output files (.ent, .ext, .ob) based on the memory content.
 *
 * This function creates the output files required by the assembler, such as the entry file (.ent),
 * the external file (.ext), and the object file (.ob). It also prints the paths of the created files.
 *
 * @param filenames The list of source filenames.
 * @param file_count The number of source files.
 * @param mem Pointer to the Memory structure.
 */
void write_output_files(const char **filenames, int file_count, Memory *mem) {
    Label *label;
    ListNode *node;
    bool has_entry = false;
    bool has_extern = false;
    char *formatted_filename = extract_and_format_filename(filenames, file_count);

    printf("Created output files:\n");

    for (label = mem->label_list; label != NULL; label = label->next) {
        if (label->entry) {
            write_to_entry_file(label->name, label->address, formatted_filename);
            has_entry = true;
        } else if (label->external) {
            write_to_extern_file(label->name, label->address, formatted_filename);
            has_extern = true;
        }
    }

    for (node = mem->instructionList; node != NULL; node = node->next) {
        write_to_object_file(node->address, node->data, formatted_filename, mem);
    }

    for (node = mem->dataList; node != NULL; node = node->next) {
        write_to_object_file(node->address, node->data, formatted_filename, mem);
    }

    if (has_entry) {
        printf("  Entry file: ./%s.ent\n", formatted_filename);
    }

    if (has_extern) {
        printf("  External file: ./%s.ext\n", formatted_filename);
    }

    printf("  Object file: ./%s.ob\n", formatted_filename);

    free(formatted_filename);
}

/**
 * @brief Writes to the entry (.ent) file for a given label.
 *
 * @param name The label name.
 * @param address The address of the label.
 * @param filename The base filename for the output file.
 */
void write_to_entry_file(char *name, int address, char *filename) {
    char filepath[MAX_FILENAME_LENGTH + 7];  /* "./" + filename + ".ent" + null terminator */
    FILE *file;

    sprintf(filepath, "./%s.ent", filename);

    file = fopen(filepath, "a");
    if (file){
        fprintf(file, "%s %03d\n", name, address);
        fclose(file);
    } else {
        add_error(ERR_FILE_NOT_FOUND, filepath, 0, NULL);
    }
}

/**
 * @brief Writes to the extern (.ext) file for a given label.
 *
 * @param name The label name.
 * @param address The address of the label.
 * @param filename The base filename for the output file.
 */
void write_to_extern_file(char *name, int address, char *filename) {
    char filepath[MAX_FILENAME_LENGTH + 7];  /* "./" + filename + ".ext" + null terminator */
    FILE *file;

    sprintf(filepath, "./%s.ext", filename);

    file = fopen(filepath, "a");
    if (file){
        fprintf(file, "%s %04d\n", name, address);
        fclose(file);
    } else {
        add_error(ERR_FILE_NOT_FOUND, filepath, 0, NULL);
    }
}

/**
 * @brief Writes to the object (.ob) file.
 *
 * @param address The memory address of the data.
 * @param data The data to write to the file.
 * @param filename The base filename for the output file.
 * @param mem Pointer to the Memory structure.
 */
void write_to_object_file(int address, int data, char *filename, Memory *mem) {
    char filepath[MAX_FILENAME_LENGTH + 7];  /* "./" + filename + ".ob" + null terminator */
    FILE *file;

    sprintf(filepath, "./%s.ob", filename);

    file = fopen(filepath, "r");
    if (file == NULL) {
        file = fopen(filepath, "a");
        if (file) {
            fprintf(file, "   %d %d\n", mem->IC, mem->DC - 100);
            fprintf(file, "%04d %05o\n", address, data);
            fclose(file);
        }
    } else {
        fclose(file);
        file = fopen(filepath, "a");
        if (file) {
            fprintf(file, "%04d %05o\n", address, data);
            fclose(file);
        }
    }
}

/**
 * @brief Adds the ".as" suffix to a filename if it does not already have it.
 *
 * @param filename The filename to modify.
 */
void add_as_suffix(char *filename) {
    if (!strstr(filename, ".as")) {
        strcat(filename, ".as");
    }
}

/**
 * @brief Prepares the filenames for processing by appending the ".as" suffix if necessary.
 *
 * @param argc The number of command-line arguments.
 * @param argv The list of command-line arguments.
 * @param filenames_ptr Pointer to the list of filenames to be populated.
 * @param file_count Pointer to the count of filenames to be populated.
 * @return True if the filenames were prepared successfully, false otherwise.
 */
bool prepare_filenames(int argc, char *argv[], const char ***filenames_ptr, int *file_count) {
    int i, j;
    char *filename_with_suffix;
    FILE *file;

    *file_count = argc - 1;
    *filenames_ptr = (const char **) malloc(*file_count * sizeof(char *));
    if (*filenames_ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory for filenames.\n");
        return false;
    }

    for (i = 1; i < argc; i++) {
        filename_with_suffix = (char *) malloc(MAX_FILENAME_LENGTH * sizeof(char));
        if (filename_with_suffix == NULL) {
            fprintf(stderr, "Failed to allocate memory for filename.\n");
            for (j = 0; j < i; j++) {
                free((void *) (*filenames_ptr)[j]);
            }
            free(*filenames_ptr);
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
                free((void *) (*filenames_ptr)[j]);
            }
            free(*filenames_ptr);
            return false;
        }
        fclose(file);
        (*filenames_ptr)[i - 1] = filename_with_suffix;
    }

    return true;
}

/**
 * @brief Frees the memory allocated for the list of filenames.
 *
 * @param filenames The list of filenames to free.
 * @param file_count The number of filenames in the list.
 */
void free_filenames(const char **filenames, int file_count) {
    int i;
    if (filenames != NULL) {
        for (i = 0; i < file_count; i++) {
            free((void *) filenames[i]);
        }
        free(filenames);
    }
}

/**
 * @brief Replaces the file extension of each filename with ".am".
 *
 * @param filenames The list of filenames to modify.
 * @param count The number of filenames in the list.
 */
void fix_filenames(const char **filenames, int count) {
    int i;
    char *dot;
    for (i = 0; i < count; i++) {
        dot = strrchr((char *)filenames[i], '.'); /* Find the last '.' in the filename */
        if (dot) {
            strcpy(dot, ".am"); /* Replace the suffix with ".am" */
        } else {
            strcat((char *)filenames[i], ".am"); /* If no dot is found, add ".am" at the end */
        }
    }
}

/**
 * @brief Creates preprocessed output files (.am) from the given contexts.
 *
 * @param file_count The number of files.
 * @param contexts The list of contexts containing preprocessed lines.
 */
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
            contexts[i].preprocessed_lines[j] = NULL;
        }
        fclose(output);
        printf("Preprocessing succeeded. Output written to %s\n", output_filename);
    }
}

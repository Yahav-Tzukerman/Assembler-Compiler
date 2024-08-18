/* Utility functions */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define INITIAL_LINE_LENGTH 128

/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * This function modifies the input string by removing any leading and trailing
 * whitespace characters, such as spaces and tabs. The resulting string is returned.
 *
 * @param str The string to trim. It is modified in place.
 * @return A pointer to the trimmed string.
 */
char* trim_whitespace(char *str) {
    char *end;

    /* Trim leading space */
    while(isspace((unsigned char)*str)){
        str++;
    }

    if(*str == 0){  /* All spaces? */
        return str;
    }

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)){
        end--;
    }

    /* Write new null terminator character */
    end[1] = '\0';

    return str;
}

/**
 * @brief Reads a line of text from a file.
 *
 * This function reads characters from a file stream until a newline or EOF is encountered.
 * It dynamically resizes the buffer as needed to accommodate the line. The caller is responsible
 * for freeing the memory allocated for the line.
 *
 * @param file The file stream to read from.
 * @return A pointer to the dynamically allocated string containing the line, or NULL if EOF is reached.
 */
char* read_line(FILE *file){
    char *line = malloc(INITIAL_LINE_LENGTH);
    int length = INITIAL_LINE_LENGTH;
    int pos = 0;
    int c;

    if (!line){
        return NULL;
    }

    while ((c = fgetc(file)) != EOF && c != '\n'){
        if (pos >= length - 1){
            length *= 2;
            line = realloc(line, length);
            if (!line){
                return NULL;
            }
        }

        line[pos++] = c;
    }

    if (c == EOF && pos == 0){
        free(line);
        return NULL;
    }

    line[pos] = '\0';
    return line;
}

/**
 * @brief Duplicates a string by dynamically allocating memory for the copy and returning it.
 *
 * This function creates a new copy of the input string in dynamically allocated memory.
 * The caller is responsible for freeing the allocated memory.
 *
 * @param str The string to duplicate.
 * @return A pointer to the newly allocated copy of the string, or NULL if the input is NULL.
 */
char* str_duplicate(const char *str) {
    size_t len;
    char *dup;
    if(str == NULL){
        return NULL;
    }
    len = strlen(str) + 1; /* Length of the source string plus null terminator. */
    dup = malloc(len); /* Allocate memory for the duplicate. */
    if (dup != NULL) {
        memcpy(dup, str, len); /* Copy the string into the new memory. */
    }
    return dup;
}

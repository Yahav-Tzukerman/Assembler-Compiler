#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define NULL_TERMINATOR '\0'

typedef int bool;
#define true 1
#define false 0

/**
 * @brief Reads a line of text from a file.
 *
 * @param file The file stream to read from.
 * @return A pointer to the dynamically allocated string containing the line, or NULL if EOF is reached.
 */
char* read_line(FILE *file);

/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * @param str The string to trim. It is modified in place.
 * @return A pointer to the trimmed string.
 */
char* trim_whitespace(char *str);

/**
 * @brief Duplicates a string by dynamically allocating memory for the copy and returning it.
 *
 * @param str The string to duplicate.
 * @return A pointer to the newly allocated copy of the string, or NULL if the input is NULL.
 */
char* str_duplicate(const char *s);

#endif /* UTILS_H */

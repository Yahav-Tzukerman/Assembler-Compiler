#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

char* read_line(FILE *file);
char* trim_whitespace(char *str);
char* str_duplicate(const char *s);

#endif /* UTILS_H */


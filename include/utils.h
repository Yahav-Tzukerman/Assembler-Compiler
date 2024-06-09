#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

char* read_line(FILE *file);
char* strtoken(char *str, const char *delim);

#endif /* UTILS_H */


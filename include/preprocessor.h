#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "utils.h"
#include <stdio.h>

#define MAX_LINE_LENGTH 256

typedef struct Macro {
    char name[32];
    char **lines;
    int line_count;
    struct Macro *next;
} Macro;

bool preprocess(const char *filename, const char *output_filename);
void add_macro(Macro *new_macro);
Macro *find_macro(const char *name);
bool process_macro_definition(FILE *input, const char *name);
void expand_macros(FILE *input, FILE *output);

#endif /* PREPROCESSOR_H */


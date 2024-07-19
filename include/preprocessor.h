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

typedef struct {
	const char *filename;
	int line_number;
	char **preprocessed_lines;
	int line_count;
	int line_capacity;
} Context;

bool preprocess(const char *filename, Context *context);
void add_macro(Macro *new_macro);
Macro *find_macro(const char *name);
bool process_macro_definition(FILE *input, char *name, Context *context);
void expand_macros(FILE *input, Context *context);
void add_preprocessed_line(Context *context, const char *line);
void free_context(Context *context);
void free_macros();

#endif /* PREPROCESSOR_H */


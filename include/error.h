#ifndef ERROR_H
#define ERROR_H

#include "utils.h"

typedef enum {
	ERR_FILE_NOT_FOUND,
	ERR_MACRO_NAME_MISSING,
	ERR_MACRO_NAME_IS_NOT_VALID,
	ERR_MEMORY_ALLOCATION_FAILED,
	/* Add more error codes as needed */
	ERR_UNKNOWN
} ErrorCode;

typedef struct {
	ErrorCode code;
	char message[256];
	char filename[256];
    	int line;
} Error;

void init_error_handling();
void add_error(ErrorCode code, const char *filename, int line, const char *detail);
void print_errors();
bool has_errors();
void free_errors();

#endif /* ERROR_H */


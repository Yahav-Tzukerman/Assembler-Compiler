/* Manages error reporting throughout the assembly process.
** Provides a centralized function to report errors consistently.
*/

#include <stdio.h>
#include "error.h"

void report_error(const char *message) {
	fprintf(stderr, "Error: %s\n", message);
}


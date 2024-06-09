#include "validations.h"
#include <string.h>
#include <ctype.h>

/* Array of valid command strings. */
const char* validCommands[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
/* Number of valid commands in the array. */
const int validCommandsCount = sizeof(validCommands) / sizeof(validCommands[0]);

bool validate_macro_name(char* macroName){
	int i;

	/* Check if the macroName is not null*/
	if (macroName == NULL){
		return false;
	}
		
	/* Check if the macroName is not a reserved command */
	for (i = 0; i < validCommandsCount; i++){
		if (strcmp(validCommands[i], macroName) == 0){
			return false;
		}
	}
	
	/* Check that the macroName starts with a letter */
	if (!isalpha(macroName[0])){
		return false;
	}

	/* Check if the macroName is a register name */
	if (strlen(macroName) == 2 && macroName[0] == 'r' && macroName[1] >= '0' && macroName[1] <= '7'){
		return false;
	}  

	return true;
}

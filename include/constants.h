#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

/* Define addressing modes */
#define UNDEFINED_MODE -1
#define IMMEDIATE_MODE 1
#define DIRECT_MODE 2
#define INDIRECT_REGISTER_MODE 4
#define DIRECT_REGISTER_MODE 8

/* Define Registers */
#define FIRST_REGISTER '1'
#define LAST_REGISTER '7'

/* Define ARE */
#define ARE_ABSOLUTE 4
#define ARE_RELOCATABLE 2
#define ARE_EXTERNAL 1

#endif /* ASSEMBLER_CONSTANTS_H */

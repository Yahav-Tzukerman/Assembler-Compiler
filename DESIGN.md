# Design and Architecture

## Overview
This document describes the design and architecture of the assembler project.

## Modules
- `assembler.c`: Coordinates the assembly process.
- `preprocessor.c`: Handles macro expansion.
- `parser.c`: Parses assembly code.
- `lexer.c`: Tokenizes the assembly code.
- `codegen.c`: Generates machine code.
- `error.c`: Manages error reporting.
- `memory.c`: Manages memory operations.
- `label.c`: Manages labels and pending writes.
- `directives.c`: Handles directives.
- `symbol_table.c`: Manages symbols.
- `instruction.c`: Handles different assembly instructions.
- `log.c`: Manages logging.
- `utils.c`: Provides utility functions.
- `output.c`: Handles output file generation.


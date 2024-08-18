CC = gcc
CFLAGS = -ansi -Wall -pedantic -Iinclude -g

OBJS = src/main.o src/assembler.o src/preprocessor.o src/utils.o src/error.o src/validations.o src/file_manager.o src/linked_list.o src/memory.o src/label.o src/operations.o src/parser.o

all: assembler

assembler: $(OBJS)
	$(CC) $(CFLAGS) -o assembler $(OBJS)

src/assembler.o: src/assembler.c include/assembler.h include/error.h include/memory.h include/parser.h include/file_manager.h
	$(CC) $(CFLAGS) -c src/assembler.c -o src/assembler.o

src/error.o: src/error.c include/error.h
	$(CC) $(CFLAGS) -c src/error.c -o src/error.o

src/file_manager.o: src/file_manager.c include/file_manager.h include/error.h
	$(CC) $(CFLAGS) -c src/file_manager.c -o src/file_manager.o

src/label.o: src/label.c include/label.h include/utils.h
	$(CC) $(CFLAGS) -c src/label.c -o src/label.o

src/linked_list.o: src/linked_list.c include/linked_list.h
	$(CC) $(CFLAGS) -c src/linked_list.c -o src/linked_list.o

src/main.o: src/main.c include/assembler.h include/preprocessor.h include/error.h include/file_manager.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/memory.o: src/memory.c include/memory.h include/utils.h
	$(CC) $(CFLAGS) -c src/memory.c -o src/memory.o

src/operations.o: src/operations.c include/operations.h
	$(CC) $(CFLAGS) -c src/operations.c -o src/operations.o

src/parser.o: src/parser.c include/parser.h include/memory.h include/utils.h include/error.h include/label.h include/operations.h include/validations.h include/constants.h
	$(CC) $(CFLAGS) -c src/parser.c -o src/parser.o

src/preprocessor.o: src/preprocessor.c include/preprocessor.h include/validations.h include/error.h
	$(CC) $(CFLAGS) -c src/preprocessor.c -o src/preprocessor.o

src/utils.o: src/utils.c include/utils.h
	$(CC) $(CFLAGS) -c src/utils.c -o src/utils.o

src/validations.o: src/validations.c include/validations.h include/preprocessor.h include/memory.h include/error.h include/constants.h
	$(CC) $(CFLAGS) -c src/validations.c -o src/validations.o

clean:
	rm -f src/*.o assembler


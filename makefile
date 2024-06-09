CC = gcc
CFLAGS = -ansi -Wall -pedantic -Iinclude

OBJS = src/main.o src/assembler.o src/preprocessor.o src/utils.o src/error.o

all: assembler

assembler: $(OBJS)
	$(CC) $(CFLAGS) -o assembler $(OBJS)

src/main.o: src/main.c include/assembler.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/assembler.o: src/assembler.c include/assembler.h include/preprocessor.h
	$(CC) $(CFLAGS) -c src/assembler.c -o src/assembler.o

src/preprocessor.o: src/preprocessor.c include/preprocessor.h include/utils.h
	$(CC) $(CFLAGS) -c src/preprocessor.c -o src/preprocessor.o

src/utils.o: src/utils.c include/utils.h
	$(CC) $(CFLAGS) -c src/utils.c -o src/utils.o

src/error.o: src/error.c include/error.h
	$(CC) $(CFLAGS) -c src/error.c -o src/error.o

clean:
	rm -f src/*.o assembler


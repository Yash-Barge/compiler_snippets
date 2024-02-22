CC = gcc
CFLAGS = -Wall -Wextra # -Werror -O2
INC_DIR = -I ./headers

stage1exe: main.o symboltable.o dfa.o buffer.o errors.o
	$(CC) $(CFLAGS) $(INC_DIR) -o stage1exe main.o symboltable.o dfa.o buffer.o errors.o
	rm -f *.o
	./stage1exe testcases/f1.txt
# all executions of ./stage1exe should be removed for final submission
main.o: main.c headers/dfa.h
	$(CC) $(CFLAGS) $(INC_DIR) -c main.c
symboltable.o: source/symboltable.c headers/symboltable.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/symboltable.c
# enumprint.o: enumprint.c enums.h
# 	$(CC) $(CFLAGS) $(INC_DIR) -c enumprint.c
dfa.o: source/dfa.c headers/dfa.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/dfa.c
errors.o: source/errors.c headers/errors.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/errors.c
buffer.o: source/buffer.c headers/buffer.h headers/errors.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/buffer.c
clean:
	rm -f *.o prog

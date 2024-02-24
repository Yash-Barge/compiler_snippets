CC = gcc
CFLAGS = -Wall -Wextra # -Werror -O2
INC_DIR = -I ./headers

stage1exe: main.o symboltable.o dfa.o buffer.o errors.o first_follow_sets.o set.o enums.o vector.o
	$(CC) $(CFLAGS) $(INC_DIR) -o stage1exe main.o symboltable.o dfa.o buffer.o errors.o first_follow_sets.o set.o enums.o vector.o
	rm -f *.o
	./stage1exe testcases/f1.txt
# all executions of ./stage1exe should be removed for final submission
main.o: main.c headers/dfa.h headers/symboltable.h headers/errors.h headers/first_follow_sets.h
	$(CC) $(CFLAGS) $(INC_DIR) -c main.c
first_follow_sets.o: source/first_follow_sets.c headers/first_follow_sets.h headers/set.h headers/vector.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/first_follow_sets.c
symboltable.o: source/symboltable.c headers/symboltable.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/symboltable.c
enums.o: source/enums.c headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/enums.c
dfa.o: source/dfa.c headers/dfa.h headers/buffer.h headers/symboltable.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/dfa.c
errors.o: source/errors.c headers/errors.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/errors.c
buffer.o: source/buffer.c headers/buffer.h headers/errors.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/buffer.c
set.o: source/set.c headers/set.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/set.c
vector.o: source/vector.c headers/vector.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/vector.c
clean:
	rm -f *.o prog

CC = gcc
CFLAGS = -Wall -Wextra # -Werror -O2
INC_DIR = -I ./headers

stage1exe: main.o symbol_table.o dfa.o buffer.o errors.o first_follow_sets.o set.o enums.o vector.o parser.o stack.o tree.o
	$(CC) $(CFLAGS) $(INC_DIR) -o stage1exe main.o symbol_table.o dfa.o buffer.o errors.o first_follow_sets.o set.o enums.o vector.o parser.o stack.o tree.o
	rm -f *.o
	./stage1exe testcases/f1.txt
# all executions of ./stage1exe should be removed for final submission
main.o: main.c headers/dfa.h headers/symbol_table.h headers/errors.h headers/first_follow_sets.h headers/parser.h
	$(CC) $(CFLAGS) $(INC_DIR) -c main.c
parser.o: source/parser.c headers/parser.h headers/dfa.h headers/first_follow_sets.h headers/stack.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/parser.c
first_follow_sets.o: source/first_follow_sets.c headers/first_follow_sets.h headers/set.h headers/vector.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/first_follow_sets.c
symbol_table.o: source/symbol_table.c headers/symbol_table.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/symbol_table.c
enums.o: source/enums.c headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/enums.c
dfa.o: source/dfa.c headers/dfa.h headers/buffer.h headers/symbol_table.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/dfa.c
errors.o: source/errors.c headers/errors.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/errors.c
buffer.o: source/buffer.c headers/buffer.h headers/errors.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/buffer.c
tree.o: source/tree.c headers/tree.h headers/vector.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/tree.c
set.o: source/set.c headers/set.h headers/enums.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/set.c
vector.o: source/vector.c headers/vector.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/vector.c
stack.o: source/stack.c headers/stack.h
	$(CC) $(CFLAGS) $(INC_DIR) -c source/stack.c
clean:
	rm -f *.o prog

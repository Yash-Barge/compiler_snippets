CC = gcc

prog: first_follow_sets.o set.o
	$(CC) -o prog first_follow_sets.o set.o
	rm -f *.o
	./prog
first_follow_sets.o: first_follow_sets.c first_follow_sets.h set.h
	$(CC) -c first_follow_sets.c
set.o: set.c set.h
	$(CC) -c set.c
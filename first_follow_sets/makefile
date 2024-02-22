CC = gcc

prog: first_follow_sets.o set.o vector.o
	$(CC) -o prog first_follow_sets.o set.o vector.o
	rm -f *.o
	./prog
first_follow_sets.o: first_follow_sets.c first_follow_sets.h set.h vector.h
	$(CC) -c first_follow_sets.c
set.o: set.c set.h
	$(CC) -c set.c
vector.o: vector.c vector.h
	$(CC) -c vector.c

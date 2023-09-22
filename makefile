CC =  gcc
CFLAGS = -Wall -Wextra -Werror -O2

prog: main.o stack.o vector.o queue.o errors.o
	$(CC) $(CFLAGS) -o prog main.o stack.o vector.o queue.o errors.o
	rm -f *.o
	./prog
test: main.o stack.o vector.o queue.o errors.o
	$(CC) $(CFLAGS) -o prog main.o stack.o vector.o queue.o errors.o
	rm -f *.o
	./prog -Tall
main.o: main.c data_structs/stack.h data_structs/vector.h data_structs/queue.h
	$(CC) $(CFLAGS) -c main.c
stack.o: data_structs/stack.c data_structs/stack.h
	$(CC) $(CFLAGS) -c data_structs/stack.c
vector.o: data_structs/vector.c data_structs/vector.h
	$(CC) $(CFLAGS) -c data_structs/vector.c
queue.o: data_structs/queue.c data_structs/queue.h
	$(CC) $(CFLAGS) -c data_structs/queue.c
errors.o: lib/errors.c lib/errors.h
	$(CC) $(CFLAGS) -c lib/errors.c
clean:
	rm -f *.o prog

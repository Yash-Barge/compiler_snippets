test: main.o stack.o
	gcc -o test main.o stack.o
	rm -f *.o
	./test
main.o: main.c data_structs/stack.h
	gcc -c main.c
stack.o: data_structs/stack.c data_structs/stack.h
	gcc -c data_structs/stack.c
clean:
	rm -f *.o test
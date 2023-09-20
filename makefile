test: main.o stack.o vector.o
	gcc -o test main.o stack.o vector.o
	rm -f *.o
	./test
main.o: main.c data_structs/stack.h data_structs/vector.h
	gcc -c main.c
stack.o: data_structs/stack.c data_structs/stack.h
	gcc -c data_structs/stack.c
vector.o: data_structs/vector.c data_structs/vector.h
	gcc -c data_structs/vector.c
clean:
	rm -f *.o test

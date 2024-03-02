CC =  gcc
# CFLAGS = -Wall -Wextra -Werror -O2

prog: main.o vector.o queue.o hashmap.o symbol_table.o regex.o # errors.o stack.o
	$(CC) $(CFLAGS) -o prog main.o vector.o queue.o hashmap.o symbol_table.o regex.o
	rm -f *.o
	./prog
test: main.o vector.o queue.o hashmap.o symbol_table.o regex.o # stack.o errors.o 
	$(CC) $(CFLAGS) -o prog main.o vector.o queue.o hashmap.o symbol_table.o regex.o
	rm -f *.o
	./prog -Tall
main.o: main.c data_structs/vector.h data_structs/queue.h data_structs/hashmap.h data_structs/symbol_table.h lexer_test/regex/regex.h # data_structs/stack.h 
	$(CC) $(CFLAGS) -c main.c
# stack.o: data_structs/stack.c data_structs/stack.h
# 	$(CC) $(CFLAGS) -c data_structs/stack.c
vector.o: data_structs/vector.c data_structs/vector.h
	$(CC) $(CFLAGS) -c data_structs/vector.c
queue.o: data_structs/queue.c data_structs/queue.h
	$(CC) $(CFLAGS) -c data_structs/queue.c
hashmap.o: data_structs/hashmap.c data_structs/hashmap.h
	$(CC) $(CFLAGS) -c data_structs/hashmap.c
regex.o: lexer_test/regex/regex.c lexer_test/regex/regex.h
	$(CC) $(CFLAGS) -c lexer_test/regex/regex.c
symbol_table.o: data_structs/symbol_table.c data_structs/symbol_table.h lexer_test/regex/regex.h
	$(CC) $(CFLAGS) -c data_structs/symbol_table.c
# errors.o: lib/errors.c lib/errors.h
# 	$(CC) $(CFLAGS) -c lib/errors.c
clean:
	rm -f *.o prog

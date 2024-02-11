CC = gcc
CFLAGS = -Wall -Wextra # -Werror -O2

prog: lexer.o symboltable.o dfa.o buffer.o
	$(CC) $(CFLAGS) -o prog lexer.o symboltable.o dfa.o buffer.o
	rm -f *.o
	./prog
lexer.o: lexer.c lexer.h dfa.h
	$(CC) $(CFLAGS) -c lexer.c
symboltable.o: symboltable.c symboltable.h
	$(CC) $(CFLAGS) -c symboltable.c
# enumprint.o: enumprint.c enums.h
# 	$(CC) $(CFLAGS) -c enumprint.c
dfa.o: dfa.c dfa.h
	$(CC) $(CFLAGS) -c dfa.c
buffer.o: buffer.c buffer.h
	$(CC) $(CFLAGS) -c buffer.c
clean:
	rm -f *.o prog

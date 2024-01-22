CC =  gcc
CFLAGS = -Wall -Wextra # -Werror  -O2

prog: lexer_test.o regex.o sample_code.txt
	$(CC) $(CFLAGS) -o prog lexer_test.o regex.o
	rm -f *.o
	./prog sample_code.txt
lexer_test.o: regex/regex.h lexer_test.c
	$(CC) $(CFLAGS) -c lexer_test.c
regex.o: regex/regex.h regex/regex.c
	$(CC) $(CFLAGS) -c regex/regex.c
clean:
	rm -f *.o prog

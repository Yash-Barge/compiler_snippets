#    GROUP NUMBER - 27
#    Shubham Gupta - 2021A7PS0468P
#    Yash Sangram Barge - 2021A7PS0006P
#    Venkatavihan Devaki - 2021A7PS0429P
#    Priyank Shethia - 2021A7PS0657P
#    Aditya Khandelwal - 2021A7PS2422P
CC = gcc

stage1exe: driver.o lexer.o parser.o others.o
	$(CC) -o stage1exe driver.o lexer.o parser.o others.o
	rm -f *.o
driver.o: driver.c lexer.h lexerDef.h parser.h parserDef.h others.h
	$(CC) -c driver.c
lexer.o: lexer.c lexer.h lexerDef.h others.h
	$(CC) -c lexer.c
parser.o: parser.c parser.h parserDef.h others.h
	$(CC) -c parser.c
others.o: others.c others.h lexerDef.h
	$(CC) -c others.c
clean:
	rm -f *.o stage1exe

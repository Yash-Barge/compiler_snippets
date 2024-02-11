#include <stdio.h>
#include <stdlib.h>

#include "dfa.h"
#include "symboltable.h"
#include "errors.h"

void lexer(char *file_name){
    IOHandler* io = createIOHandler(file_name);
    
    Table symboltable = createtable();
    populate(symboltable);

    while (!io->inputFin)
        runDFA(io, symboltable);

    closeHandler(io);

    if (get_error_count()) {
        fprintf(stderr, "\033[1;31merror: \033[0mLexing of file %s ended with \033[1;31m%d lexer error(s)\033[0m \nTerminating compilation...\n", file_name, get_error_count());
        exit(0);
    }

    printf("Lexing of file %s successful!\n", file_name);

    return;
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        error("Invalid number of arguments! Execute program followed by source file!\n");
        return 1;
    }

    lexer(argv[1]);

    return 0;
}
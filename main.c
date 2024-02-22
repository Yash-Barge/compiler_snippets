#include <stdio.h>
#include <stdlib.h>

#include "dfa.h"
#include "symboltable.h"
#include "errors.h"

void print_source_without_comments(char *file_name) {
    IOHandler *io = createIOHandler(file_name);

    while (!io->inputFin) {
        char c = getChar(io, true);

        if (c == '%')
            while (c != '\n' && c != '\0')
                c = getChar(io, true);

        printf("%c", c);
    }

    printf("\n");

    closeHandler(io);

    return;
}

void lexer(char *file_name) {
    IOHandler *io = createIOHandler(file_name);

    Table symboltable = createtable();
    populate(symboltable);

    while (!io->inputFin)
        runDFA(io, symboltable);

    closeHandler(io);

    if (get_error_count())
        fprintf(stderr, "\033[1;31merror: \033[0mLexing of file %s failed with \033[1;31m%d lexer error(s)\033[0m\n", file_name, get_error_count());
    else
        printf("Lexing of file %s successful!\n", file_name);
    
    reset_error_count();

    return;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { // should eventually change to 3, to put parser output in a file
        error("Invalid number of arguments! Execute program followed by source file!\n");
        return 1;
    }

    int input = -1;

    while (input != 0) {
        printf("Enter a number from 0-4: ");
        scanf("%d", &input);

        switch (input) {
        case 0:
            printf("Exiting program...\n");
            break;
        case 1:
            print_source_without_comments(argv[1]);
            break;
        case 2:
            lexer(argv[1]);
            break;
        case 3:
            printf("TODO...\n");
            break;
        case 4:
            printf("TODO...\n");
            break;
        default:
            printf("Unrecognized input %d!\n", input);
            continue;
        }

    }

    return 0;
}

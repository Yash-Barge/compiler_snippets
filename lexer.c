#include "dfa.h"
#include "symboltable.h"
#include <stdio.h>

void lexer(char * file_name){
    IOHandler* io = createIOHandler(file_name);
    Table symboltable = createtable();
    populate(symboltable);
    while(io->inputFin==false){
        runDFA(io, symboltable);
    }
}

int main(){
    lexer("f1.txt");
    return 0;
}
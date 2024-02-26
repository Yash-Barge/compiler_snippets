#ifndef H_DFA
#define H_DFA

#include <stdbool.h>

#include "buffer.h"
#include "symboltable.h"
#include "enums.h"

typedef struct token TOKEN;

struct token{
    enum terminals token;
    char *lexeme;
    int lineNumber;
    bool isError;
};

TOKEN* createToken();
TOKEN* runDFA(IOHandler *io, Table symboltable);
void printToken(TOKEN * token);
void free_token(TOKEN **p_token);

#endif

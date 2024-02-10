#include "buffer.h"
#include "symboltable.h"
#include <stdbool.h>
typedef struct token TOKEN;
struct token{
    enum token_names token;
    char *lexeme;
    int lineNumber;
    bool isError;
};
TOKEN* createToken();
TOKEN* runDFA(IOHandler *io, Table symboltable);
void printToken(TOKEN * token);
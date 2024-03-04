#ifndef H_DFA
#define H_DFA

#include <stdbool.h>

#include "buffer.h"
#include "symbol_table.h"
#include "enums.h"
#include "errors.h"

typedef struct token TOKEN;

struct token {
    struct st_data *data;
    int lineNumber;
};

TOKEN* runDFA(IOHandler *io, struct symbol_table *st);
void printToken(TOKEN *token);

#endif

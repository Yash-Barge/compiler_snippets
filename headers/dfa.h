#ifndef H_DFA
#define H_DFA

#include <stdbool.h>

#include "buffer.h"
#include "symbol_table.h"
#include "enums.h"

typedef struct token TOKEN;
/**
 * @brief defines token
 * 
 */
struct token {
    struct st_data *data; //token data
    int lineNumber;
};

TOKEN* runDFA(IOHandler *io, struct symbol_table *st);
void printToken(TOKEN *token);

#endif

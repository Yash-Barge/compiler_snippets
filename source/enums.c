#include <string.h>

#include "enums.h"

#define GENERATE_STRINGS(STR) #STR,

const char *const TOK_STRING[] = {
    FOREACH_TOK(GENERATE_STRINGS)
};


enum terminals tok_enumify(const char *restrict const tok) {
    int index = -1;

    for (int i = 0; i < TK_COUNT; i++)
        if (!strcmp(tok, TOK_STRING[i])) {
            index = i;
            break;
        }
    
    return index;
}

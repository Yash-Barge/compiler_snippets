#include <string.h>

#include "enums.h"

#define GENERATE_STRINGS(STR) #STR,

/**
 * @brief Array that contains string-versions of the token types, generated
 * maintaining sychronization using FOREACH_TOK macro  
 * 
 */
const char *const TOK_STRING[] = {
    FOREACH_TOK(GENERATE_STRINGS)
};

/**
 * @brief Takes a string and converts it to an enum 
 * 
 * @param tok token
 * @return enum terminals 
 */
enum terminals tok_enumify(const char *restrict const tok) {
    int index = -1;

    for (int i = 0; i < TK_COUNT; i++)
        if (!strcmp(tok, TOK_STRING[i])) {
            index = i;
            break;
        }
    
    return index;
}

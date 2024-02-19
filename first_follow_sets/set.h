#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#pragma once

// #define FOREACH_TOK(TOK)            \
//         TOK(TK_ASSIGNOP)            \
//         TOK(TK_COMMENT)             \
//         TOK(TK_FIELDID)             \
//         TOK(TK_ID)                  \
//         TOK(TK_NUM)                 \
//         TOK(TK_RNUM)                \
//         TOK(TK_FUNID)               \
//         TOK(TK_RUID)                \
//         TOK(TK_WITH)                \
//         TOK(TK_PARAMETERS)          \
//         TOK(TK_END)                 \
//         TOK(TK_WHILE)               \
//         TOK(TK_UNION)               \
//         TOK(TK_ENDUNION)            \
//         TOK(TK_DEFINETYPE)          \
//         TOK(TK_AS)                  \
//         TOK(TK_TYPE)                \
//         TOK(TK_MAIN)                \
//         TOK(TK_GLOBAL)              \
//         TOK(TK_PARAMETER)           \
//         TOK(TK_LIST)                \
//         TOK(TK_SQL)                 \
//         TOK(TK_SQR)                 \
//         TOK(TK_INPUT)               \
//         TOK(TK_OUTPUT)              \
//         TOK(TK_INT)                 \
//         TOK(TK_REAL)                \
//         TOK(TK_COMMA)               \
//         TOK(TK_SEM)                 \
//         TOK(TK_COLON)               \
//         TOK(TK_DOT)                 \
//         TOK(TK_ENDWHILE)            \
//         TOK(TK_OP)                  \
//         TOK(TK_CL)                  \
//         TOK(TK_IF)                  \
//         TOK(TK_THEN)                \
//         TOK(TK_ENDIF)               \
//         TOK(TK_READ)                \
//         TOK(TK_WRITE)               \
//         TOK(TK_RETURN)              \
//         TOK(TK_PLUS)                \
//         TOK(TK_MINUS)               \
//         TOK(TK_MUL)                 \
//         TOK(TK_DIV)                 \
//         TOK(TK_CALL)                \
//         TOK(TK_RECORD)              \
//         TOK(TK_ENDRECORD)           \
//         TOK(TK_ELSE)                \
//         TOK(TK_AND)                 \
//         TOK(TK_OR)                  \
//         TOK(TK_NOT)                 \
//         TOK(TK_LT)                  \
//         TOK(TK_LE)                  \
//         TOK(TK_EQ)                  \
//         TOK(TK_GT)                  \
//         TOK(TK_GE)                  \
//         TOK(TK_NE)                  \
//         TOK(TK_EPSILON)             \
         
// #define GENERATE_ENUM(TOK) TOK, 
// #define GENERATE_STRINGS(STR) #STR,

// enum TOK_ENUM {
//     FOREACH_TOK(GENERATE_ENUM)
// };

// static const char* TOK_STRING[] = {
//     FOREACH_TOK(GENERATE_STRINGS)
// };

struct set;

enum terminals {
    TK_ASSIGNOP,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EO,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_EPSILON,
    TK_COUNT
};


struct set_lib {
    struct set *(*const new)(void);
    int (*const size)(struct set *);
    void (*const insert)(struct set *, enum terminals);
    void (*const push_back)(struct set *, int);
    void (*const push_front)(struct set *, int);
    void (*const print)(struct set *);
    void (*const free)(struct set **);
};

extern const struct set_lib Set;

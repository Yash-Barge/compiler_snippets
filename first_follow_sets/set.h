#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define GENERATE_ENUM(TOK) TOK, 

#define FOREACH_TOK(TOK) \
    TOK(TK_ASSIGNOP) \
    TOK(TK_WITH) \
    TOK(TK_PARAMETERS) \
    TOK(TK_END) \
    TOK(TK_WHILE) \
    TOK(TK_UNION) \
    TOK(TK_ENDUNION) \
    TOK(TK_DEFINETYPE) \
    TOK(TK_AS) \
    TOK(TK_TYPE) \
    TOK(TK_MAIN) \
    TOK(TK_GLOBAL) \
    TOK(TK_PARAMETER) \
    TOK(TK_LIST) \
    TOK(TK_SQL) \
    TOK(TK_SQR) \
    TOK(TK_INPUT) \
    TOK(TK_OUTPUT) \
    TOK(TK_INT) \
    TOK(TK_REAL) \
    TOK(TK_COMMA) \
    TOK(TK_SEM) \
    TOK(TK_COLON) \
    TOK(TK_DOT) \
    TOK(TK_ENDWHILE) \
    TOK(TK_OP) \
    TOK(TK_CL) \
    TOK(TK_IF) \
    TOK(TK_THEN) \
    TOK(TK_ENDIF) \
    TOK(TK_READ) \
    TOK(TK_WRITE) \
    TOK(TK_RETURN) \
    TOK(TK_PLUS) \
    TOK(TK_MINUS) \
    TOK(TK_MUL) \
    TOK(TK_DIV) \
    TOK(TK_CALL) \
    TOK(TK_RECORD) \
    TOK(TK_ENDRECORD) \
    TOK(TK_ELSE) \
    TOK(TK_AND) \
    TOK(TK_OR) \
    TOK(TK_NOT) \
    TOK(TK_LT) \
    TOK(TK_LE) \
    TOK(TK_EO) \
    TOK(TK_GT) \
    TOK(TK_GE) \
    TOK(TK_NE) \
    TOK(TK_EPSILON) \
    TOK(TK_COUNT)

enum terminals {
    FOREACH_TOK(GENERATE_ENUM)
};

extern const char *const TOK_STRING[];

struct set;

struct set_lib {
    struct set *(*const new)(void);
    int (*const size)(struct set *);
    void (*const insert)(struct set *, enum terminals);
    void (*const push_back)(struct set *, int);
    void (*const push_front)(struct set *, int);
    int (*const search)(struct set *, int);
    void (*const print)(struct set *);
    void (*const free)(struct set **);
};

extern const struct set_lib Set;

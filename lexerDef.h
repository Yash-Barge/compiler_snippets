/*
    GROUP NUMBER - 27
    Shubham Gupta - 2021A7PS0468P
    Yash Sangram Barge - 2021A7PS0006P
    Venkatavihan Devaki - 2021A7PS0429P
    Priyank Shethia - 2021A7PS0657P
    Aditya Khandelwal - 2021A7PS2422P
*/

#pragma once

#include <stdbool.h>

#define GENERATE_ENUM(TOK) TOK,

#define FOREACH_TOK(TOK) \
    TOK(TK_ASSIGNOP) \
    TOK(TK_COMMENT) \
    TOK(TK_FIELDID) \
    TOK(TK_ID) \
    TOK(TK_NUM) \
    TOK(TK_RNUM) \
    TOK(TK_FUNID) \
    TOK(TK_RUID) \
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
    TOK(TK_EQ) \
    TOK(TK_GT) \
    TOK(TK_GE) \
    TOK(TK_NE) \
    TOK(TK_EPSILON) \
    TOK(TK_COUNT)

enum terminals {
    FOREACH_TOK(GENERATE_ENUM)
};

extern const char *const TOK_STRING[];

enum terminals tok_enumify(const char *restrict const tok);

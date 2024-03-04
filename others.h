/*
    GROUP NUMBER - 27
    Shubham Gupta - 2021A7PS0468P
    Yash Sangram Barge - 2021A7PS0006P
    Venkatavihan Devaki - 2021A7PS0429P
    Priyank Shethia - 2021A7PS0657P
    Aditya Khandelwal - 2021A7PS2422P
*/


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "lexerDef.h"

/* TOKEN DEFINITION */

typedef struct token TOKEN;
/**
 * @brief defines token
 * 
 */
struct token {
    struct st_data *data; //token data
    int lineNumber;
};

/* ERROR HANDLING FUNCTIONS */

void error(const char *fmt, ...);
void lexer_error(const char *fmt, ...);
void parser_error(const char *fmt, ...);
int get_lexer_error_count(void);
int get_parser_error_count(void);
void reset_error_count(void);


/* SET ADT */

struct set;

struct set_lib {
    struct set *(*const new)(void);
    int (*const size)(struct set *);
    void (*const insert)(struct set *, enum terminals);
    int (*const at)(struct set *, int);
    int (*const search)(struct set *, int);
    void (*const print)(struct set *);
    void (*const free)(struct set **);
};

extern const struct set_lib Set;


/* STACK ADT*/

struct stack;

struct stack_lib {
    struct stack *(*const new)(void);
    void (*const push)(struct stack *, int);
    int (*const pop)(struct stack *);
    int (*const is_empty)(struct stack *);
    int (*const top)(struct stack *);
    void (*const free)(struct stack **);
};

extern const struct stack_lib Stack;


/* SYMBOL TABLE */

struct symbol_table;

typedef union {
    long long intVal;
    double floatVal;
} dataElements;

// Data of the lexeme, which includes the lexeme's name and the type of token
struct st_data {
    char* stringLexeme;
    dataElements lexeme;
    enum terminals token_type;
};

struct symbol_table_lib {
    struct symbol_table* (*const new)(int);
    struct st_data *(*const insert)(struct symbol_table *, char *, enum terminals);
    struct st_data *(*const search)(struct symbol_table *, char *, enum terminals);
    struct st_data *(*const search_keyword)(struct symbol_table *, char *);
    struct symbol_table* (*const init)(void);
    void (*const free)(struct symbol_table **);
};

extern const struct symbol_table_lib SymbolTable;


/* VECTOR ADT */

struct vector_int;
struct vector_string;

struct vector_int_lib {
    struct vector_int *(*const new)(void);
    int (*const size)(struct vector_int *);
    void (*const insert)(struct vector_int *, int, int);
    int (*const index_of)(struct vector_int *, int);
    int (*const erase)(struct vector_int *, int);
    void (*const push_back)(struct vector_int *, int);
    int (*const pop_back)(struct vector_int *);
    void (*const push_front)(struct vector_int *, int);
    int (*const pop_front)(struct vector_int *);
    int (*const at)(struct vector_int *, int);
    void (*const print)(struct vector_int *);
    void (*const free)(struct vector_int **);
};

struct vector_string_lib {
    struct vector_string *(*const new)(void);
    int (*const size)(struct vector_string *);
    void (*const insert)(struct vector_string *, int, char *);
    char *(*const erase)(struct vector_string *, int);
    void (*const push_back)(struct vector_string *, char *);
    char *(*const pop_back)(struct vector_string *);
    void (*const push_front)(struct vector_string *, char *);
    char *(*const pop_front)(struct vector_string *);
    const char *(*const at)(struct vector_string *, int);
    void (*const print)(struct vector_string *);
    void (*const free)(struct vector_string **);
};

extern const struct vector_int_lib VectorInt;
extern const struct vector_string_lib VectorString;



/* TREE ADT */

struct tree_node {
    int data;
    int children_count;
    struct tree_node *parent;
    struct tree_node **children;
    int line_number;
    char *lexeme;
    dataElements lex_data;
};

struct tree_lib {
    struct tree_node *(*const new)(int);
    void (*const insert_children)(struct tree_node *parent, struct vector_int *rhs_prod_rule);
    void (*const free)(struct tree_node **);
};

extern const struct tree_lib Tree;

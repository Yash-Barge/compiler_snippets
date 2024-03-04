#pragma once

#include "enums.h"

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
    struct symbol_table* (*const init)();
    void (*const free)(struct symbol_table **);
};

extern const struct symbol_table_lib SymbolTable;

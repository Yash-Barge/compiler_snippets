#pragma once

struct symbol_table;

struct symbol_table_lib {
    struct symbol_table* (*const new)(int);
    void (*const insert)(struct symbol_table **, char*);
    int (*const search)(struct symbol_table *, char*);
    struct symbol_table* (*const init)();
    void (*const free)(struct symbol_table **);
};

extern const struct symbol_table_lib SymbolTable;

#pragma once

#include "vector.h"
#include "symbol_table.h"

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

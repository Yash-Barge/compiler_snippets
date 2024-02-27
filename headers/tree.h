#pragma once

#include "vector.h"

struct tree_node {
    int data;
    int children_count;
    struct tree_node *parent;
    struct tree_node *children;
};

struct tree_lib {
    struct tree_node *(*const new)(int);
    void (*const insert)(struct tree_node *parent, struct vector_int *rhs_prod_rule);
    void (*const free)(struct tree_node **);
};

extern const struct tree_lib Tree;

#pragma once

struct tree;
struct node;

struct tree_lib {
    struct tree *(*const new)(void);
    void (*const free)(struct tree **);
};

extern const struct tree_lib Tree;

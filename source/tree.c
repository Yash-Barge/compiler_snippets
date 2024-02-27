#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"

struct tree_node *tree_node_new(int data) {
    struct tree_node *temp = malloc(sizeof(*temp));
    temp->data = data;
    temp->children_count = 0;
    temp->parent = NULL;
    temp->children = NULL;
    
    return temp;
}

void tree_node_insert(struct tree_node *parent, struct vector_int *rhs_prod_rule) {
    parent->children_count = VectorInt.size(rhs_prod_rule);
    parent->children = malloc(sizeof(*parent) * parent->children_count);

    for (int i = 0; i < parent->children_count; i++) {
        parent->children[i].parent = parent;
        parent->children[i].data = VectorInt.at(rhs_prod_rule, i);
        parent->children[i].children_count = 0;
        parent->children[i].children = NULL;
    }

    return;
}

void tree_free_helper(struct tree_node *t) {
    assert(t != NULL);

    for (int i = 0; i < t->children_count; i++)
        tree_free_helper(&t->children[i]);
    
    if (t->children_count)
        free(t->children);

    return;
}

void tree_free(struct tree_node** p_tree) {
    assert(p_tree != NULL);
    assert(*p_tree != NULL);

    struct tree_node *t = *p_tree;

    for (int i = 0; i < t->children_count; i++)
        tree_free_helper(&t->children[i]);

    if (t->children_count)
        free(t->children);
    
    free(t);

    *p_tree = NULL;

    return;
}

const struct tree_lib Tree = { .new = tree_node_new, .insert = tree_node_insert, .free = tree_free };

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
    temp->line_number = 0;
    temp->lexeme = NULL;
    temp->lex_data.intVal = 0;
    
    return temp;
}

void tree_node_insert_children(struct tree_node *parent, struct vector_int *rhs_prod_rule) {
    parent->children_count = VectorInt.size(rhs_prod_rule);
    parent->children = malloc(sizeof(struct tree_node *) * parent->children_count);

    for (int i = 0; i < parent->children_count; i++) {
        parent->children[i] = malloc(sizeof(struct tree_node));
        parent->children[i]->parent = parent;
        parent->children[i]->data = VectorInt.at(rhs_prod_rule, i);
        parent->children[i]->children_count = 0;
        parent->children[i]->children = NULL;
        parent->children[i]->line_number = 0;
        parent->children[i]->lexeme = NULL;
        parent->children[i]->lex_data.intVal = 0;
    }

    return;
}

void tree_free(struct tree_node** p_tree) {
    assert(p_tree != NULL);
    assert(*p_tree != NULL);

    struct tree_node *t = *p_tree;

    for (int i = 0; i < t->children_count; i++)
        tree_free(&t->children[i]);

    if (t->children_count)
        free(t->children);
    
    free(t->lexeme);
    free(t);

    *p_tree = NULL;

    return;
}

const struct tree_lib Tree = { .new = tree_node_new, .insert_children = tree_node_insert_children, .free = tree_free };

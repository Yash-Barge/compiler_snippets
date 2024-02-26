#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <vector.h>


struct node {
    int data;
    struct node* parent;
    struct node** children;
};

struct tree {
    struct node* root;
};

struct node* tree_node_new_without_data(void) {
    struct node* temp = malloc(sizeof(*temp));
    temp->data = -1;
    temp->parent = NULL;
    temp->children = VectorNodes.new();
    
    return temp;
}

struct node* tree_node_new_with_data(int data) {
    struct node* temp = malloc(sizeof(*temp));
    temp->data = data;
    temp->parent = NULL;
    temp->children = VectorNodes.new();
    
    return temp;
}

struct tree* tree_new(void) {
    struct tree* temp = malloc(sizeof(*temp));
    temp->root = tree_node_new_without_data();

    return temp;
}

void tree_free(struct node* t){
    if(t == NULL) return;
    for(int i=0; i < VectorInt.size(t->children); i++){
        tree_free(t->children[i]);
    }
    free(t);
}

const struct tree_lib Tree = { .new = tree_new, .free = tree_free };


#include <stdlib.h>
#include <assert.h>

#include "stack.h"

//defining node in stack
struct node {
    int data;
    struct node *next;
};

/**
 * @brief creates new node
 * 
 * @param data 
 * @return struct node* 
 */
struct node *new_node(int data) {
    struct node *n = malloc(sizeof(struct node));
    *n = (struct node) { .data = data, .next = NULL };
    return n;
}

//defines stack 
struct stack {
    struct node *top;
};

/**
 * @brief creates new stack
 * 
 * @return struct stack* 
 */
struct stack *new_stack(void) {
    struct stack *s = malloc(sizeof(struct stack));
    *s = (struct stack) { .top = NULL };
    return s;
}

/**
 * @brief pushes data onto stack
 * 
 * @param s 
 * @param data 
 */
void stack_push(struct stack *s, int data) {
    struct node *n = new_node(data);

    n->next = s->top;
    s->top = n;

    return;
}

/**
 * @brief pops data from stack
 * 
 * @param s 
 * @return int 
 */
int stack_pop(struct stack *s) {
    assert(s != NULL);

    struct node *temp = s->top;
    s->top = s->top->next;

    int temp_data = temp->data;
    free(temp);

    return temp_data;
}

int stack_is_empty(struct stack *s) {
    return s->top == NULL;
}

int stack_top(struct stack *s) {
    assert(s != NULL);
    return s->top->data;
}

void stack_free(struct stack **p_s) {
    assert(p_s != NULL);
    assert(*p_s != NULL);
    
    struct stack *s = *p_s;

    while (!stack_is_empty(s))
        stack_pop(s);
    
    free(s);
    *p_s = NULL;

    return;
}

const struct stack_lib Stack = { .new = new_stack, .push = stack_push, .pop = stack_pop, .is_empty = stack_is_empty, .top = stack_top, .free = stack_free };

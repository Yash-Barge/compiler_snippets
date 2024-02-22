#include <stdlib.h>
#include <assert.h>

#include "queue.h"

struct node {
    int data;
    struct node *next;
};

struct node *new_node_queue(int data) {
    struct node *n = malloc(sizeof(struct node));
    *n = (struct node) { .data = data, .next = NULL };
    return n;
}

struct queue {
    struct node *front;
    struct node *end;
};

struct queue *new_queue(void) {
    struct queue *q = malloc(sizeof(struct queue));
    *q = (struct queue) { .front = NULL , .end = NULL};
    return q;
}

void queue_push(struct queue *q, int data) {
    struct node *n = new_node_queue(data);
    if(!q->front && !q->end){
        q->front = n;
        q->end = n;
        return;
    }
    q->end->next = n;
    q->end = q->end->next;
    return;
}

int queue_pop(struct queue *q) {
    assert(q != NULL);
    assert(q->front != NULL);

    struct node* old = q->front;
    int old_data = old->data;
    q->front = q->front->next;
    free(old);
    return old_data;
}

int queue_is_empty(struct queue *q) {
    return q->front == NULL;
}

int queue_front(struct queue *q) {
    assert(q != NULL);
    return q->front->data;
}

void queue_free(struct queue **p_q) {
    assert(p_q != NULL);
    assert(*p_q != NULL);

    struct queue *q = *p_q;
    
    if(q->front == NULL){
        free(q);
        return;
    }
    struct node* ptr = q->front;

    while(ptr != NULL){
        struct node* next = ptr->next;
        free(ptr);
        ptr = next;
    }
    free(q);
    return;
}

const struct queue_lib Queue = { .new = new_queue, .push = queue_push, .pop = queue_pop, .is_empty = queue_is_empty, .front = queue_front, .free = queue_free };

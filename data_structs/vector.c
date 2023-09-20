#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vector.h"

struct vector {
    int *vector;
    int size;
    int capacity;
};

struct vector *new_vi(void) {
    struct vector *vec = malloc(sizeof(struct vector));
    *vec = (struct vector) { .size = 0, .capacity = 1 };
    vec->vector = malloc(sizeof(int) * vec->capacity);

    return vec;
}

void grow_vector(struct vector *vec) {
    vec->vector = realloc(vec->vector, sizeof(int) * vec->capacity * 2);
    vec->capacity *= 2;

    return;
} 

void vi_insert(struct vector *vec, int index, int data) {
    assert(vec != NULL);
    assert(index >= 0 && index <= vec->size);

    if(vec->size == vec->capacity)
        grow_vector(vec);

    vec->size++;

    for (int i = vec->size - 1; i > index; i--)
        vec->vector[i] = vec->vector[i - 1];

    vec->vector[index] = data;

    return;
}

int vi_erase(struct vector *vec, int index) {
    assert(vec != NULL);
    assert(index >= 0 && index < vec->size);

    int val = vec->vector[index];

    for(int i = index; i < vec->size - 1; i++)
        vec->vector[i] = vec->vector[i + 1];

    vec->size--;

    return val;
}

int vi_pop_back(struct vector *vec) {
    return vi_erase(vec, vec->size-1); 
}

int vi_pop_front(struct vector *vec) {
    return vi_erase(vec, 0); 
}

void vi_push_back(struct vector *vec, int val) {
    return vi_insert(vec, vec->size, val); 
}

void vi_push_front(struct vector *vec, int val) {
    return vi_insert(vec, 0, val); 
}

void vi_print(struct vector *vec) {
    assert(vec != NULL);

    for(int i = 0; i < vec->size; i++)
        printf("%d ", vec->vector[i]);
    printf("\n");

    return;
}

void vi_free(struct vector **p_vec) {
    assert(p_vec != NULL);
    assert(*p_vec != NULL);

    struct vector *vec = *p_vec;
    free(vec->vector);
    free(vec);
    *p_vec = NULL;

    return;
}

const struct vector_lib Vector = { .new = new_vi, .insert = vi_insert, .erase = vi_erase, .push_back = vi_push_back, .push_front = vi_push_front, .pop_back = vi_pop_back, .pop_front = vi_pop_front, .print = vi_print, .free = vi_free };

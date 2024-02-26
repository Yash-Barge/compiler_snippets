#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "vector.h"

/* Vector of Ints */

struct vector_int {
    int *vector;
    int size;
    int capacity;
};

struct vector_int *vint_new(void) {
    struct vector_int *vec = malloc(sizeof(*vec));
    *vec = (struct vector_int) { .size = 0, .capacity = 1 };
    vec->vector = malloc(sizeof(*vec->vector) * vec->capacity);

    return vec;
}

void vint_grow_vector(struct vector_int *vec) {
    vec->vector = realloc(vec->vector, sizeof(*vec->vector) * vec->capacity * 2);
    vec->capacity *= 2;

    return;
} 

int vint_size(struct vector_int *vec){
    assert(vec != NULL);
    return vec->size;
}

void vint_insert(struct vector_int *vec, int index, int data) {
    assert(vec != NULL);
    assert(index >= 0 && index <= vec->size);

    if(vec->size == vec->capacity)
        vint_grow_vector(vec);

    memmove(vec->vector + index + 1, vec->vector + index, sizeof(*vec->vector) * (vec->size++ - index));

    vec->vector[index] = data;

    return;
}

int vint_index_of(struct vector_int *vec, int val){
    assert(vec != NULL);
    for(int i = 0; i < vec->size; i++)
        if (vec->vector[i] == val)
            return i;
    return -1;
}


int vint_erase(struct vector_int *vec, int index) {
    assert(vec != NULL);
    assert(index >= 0 && index < vec->size);

    int val = vec->vector[index];

    memmove(vec->vector + index, vec->vector + index + 1, sizeof(*vec->vector) * (--vec->size - index));

    return val;
}

int vint_pop_back(struct vector_int *vec) {
    return vint_erase(vec, vec->size-1); 
}

int vint_pop_front(struct vector_int *vec) {
    return vint_erase(vec, 0); 
}

void vint_push_back(struct vector_int *vec, int val) {
    return vint_insert(vec, vec->size, val); 
}

void vint_push_front(struct vector_int *vec, int val) {
    return vint_insert(vec, 0, val); 
}

int vint_at(struct vector_int *vec, int index) {
    return vec->vector[index];
}

void vint_print(struct vector_int *vec) {
    assert(vec != NULL);

    for(int i = 0; i < vec->size; i++)
        printf("%d ", vec->vector[i]);
    printf("\n");

    return;
}

void vint_free(struct vector_int **p_vec) {
    assert(p_vec != NULL);
    assert(*p_vec != NULL);

    struct vector_int *vec = *p_vec;

    free(vec->vector);
    free(vec);
    *p_vec = NULL;

    return;
}

const struct vector_int_lib VectorInt = { .new = vint_new, .size = vint_size, .insert = vint_insert, .index_of = vint_index_of, .erase = vint_erase, .push_back = vint_push_back, .push_front = vint_push_front, .pop_back = vint_pop_back, .pop_front = vint_pop_front, .at = vint_at, .print = vint_print, .free = vint_free };

/* Vector for strings */

struct vector_string {
    char **vector;
    int size;
    int capacity;
};

struct vector_string *vstring_new(void) {
    struct vector_string *vec = malloc(sizeof(*vec));
    *vec = (struct vector_string) { .size = 0, .capacity = 1 };
    vec->vector = malloc(sizeof(*vec->vector) * vec->capacity);

    return vec;
}

void vstring_grow_vector(struct vector_string *vec) {
    vec->vector = realloc(vec->vector, sizeof(*vec->vector) * vec->capacity * 2);
    vec->capacity *= 2;

    return;
} 

int vstring_size(struct vector_string *vec){
    assert(vec != NULL);
    return vec->size;
}

void vstring_insert(struct vector_string *vec, int index, char *data) {
    assert(vec != NULL);
    assert(index >= 0 && index <= vec->size);

    if(vec->size == vec->capacity)
        vstring_grow_vector(vec);

    memmove(vec->vector + index + 1, vec->vector + index, sizeof(*vec->vector) * (vec->size++ - index));

    vec->vector[index] = malloc(sizeof(char) * (strlen(data) + 1));
    strcpy(vec->vector[index], data);

    return;
}

char *vstring_erase(struct vector_string *vec, int index) {
    assert(vec != NULL);
    assert(index >= 0 && index < vec->size);

    char *val = vec->vector[index];

    memmove(vec->vector + index, vec->vector + index + 1, sizeof(*vec->vector) * (--vec->size - index));

    return val;
}

char *vstring_pop_back(struct vector_string *vec) {
    return vstring_erase(vec, vec->size-1); 
}

char *vstring_pop_front(struct vector_string *vec) {
    return vstring_erase(vec, 0); 
}

void vstring_push_back(struct vector_string *vec, char *val) {
    return vstring_insert(vec, vec->size, val); 
}

void vstring_push_front(struct vector_string *vec, char *val) {
    return vstring_insert(vec, 0, val); 
}

const char *vstring_at(struct vector_string *vec, int index) {
    return vec->vector[index];
}

void vstring_print(struct vector_string *vec) {
    assert(vec != NULL);

    for(int i = 0; i < vec->size; i++)
        printf("%s\n", vec->vector[i]);
    printf("\n");

    return;
}

void vstring_free(struct vector_string **p_vec) {
    assert(p_vec != NULL);
    assert(*p_vec != NULL);

    struct vector_string *vec = *p_vec;

    for (int i = 0; i < vec->size; i++)
        free(vec->vector[i]);

    free(vec->vector);
    free(vec);
    *p_vec = NULL;

    return;
}

const struct vector_string_lib VectorString = { .new = vstring_new, .size = vstring_size, .insert = vstring_insert, .erase = vstring_erase, .push_back = vstring_push_back, .push_front = vstring_push_front, .pop_back = vstring_pop_back, .pop_front = vstring_pop_front, .at = vstring_at, .print = vstring_print, .free = vstring_free };

/* Children of the Tree of Parse tree */

struct vector_tree {
    void **children;
    int size;
    int capacity;
};

struct vector_tree *vtree_new(void) {
    struct vector_tree *vec = malloc(sizeof(*vec));
    *vec = (struct vector_tree) { .size = 0, .capacity = 1 };
    vec->children = malloc(sizeof(*vec->children) * vec->capacity);

    return vec;
}

void vtree_grow_vector(struct vector_tree *vec) {
    vec->children = realloc(vec->children, sizeof(*vec->children) * vec->capacity * 2);
    vec->capacity *= 2;

    return;
} 

int vtree_size(struct vector_tree *vec){
    assert(vec != NULL);
    return vec->size;
}

void vtree_insert(struct vector_tree *vec, int index, char *data) {
    assert(vec != NULL);
    assert(index >= 0 && index <= vec->size);

    if(vec->size == vec->capacity)
        vtree_grow_vector(vec);

    memmove(vec->children + index + 1, vec->children + index, sizeof(*vec->children) * (vec->size++ - index));

    vec->children[index] = malloc(sizeof(char) * (strlen(data) + 1));
    strcpy(vec->children[index], data);

    return;
}

char *vtree_erase(struct vector_tree *vec, int index) {
    assert(vec != NULL);
    assert(index >= 0 && index < vec->size);

    char *val = vec->children[index];

    memmove(vec->children + index, vec->children + index + 1, sizeof(*vec->children) * (--vec->size - index));

    return val;
}

char *vtree_pop_back(struct vector_tree *vec) {
    return vtree_erase(vec, vec->size-1); 
}

char *vtree_pop_front(struct vector_tree *vec) {
    return vtree_erase(vec, 0); 
}

void vtree_push_back(struct vector_tree *vec, char *val) {
    return vtree_insert(vec, vec->size, val); 
}

void vtree_push_front(struct vector_tree *vec, char *val) {
    return vtree_insert(vec, 0, val); 
}

const char *vtree_at(struct vector_tree *vec, int index) {
    return vec->children[index];
}

void vtree_print(struct vector_tree *vec) {
    assert(vec != NULL);

    for(int i = 0; i < vec->size; i++)
        printf("%s\n", vec->children[i]);
    printf("\n");

    return;
}

void vtree_free(struct vector_tree **p_vec) {
    assert(p_vec != NULL);
    assert(*p_vec != NULL);

    struct vector_tree *vec = *p_vec;

    for (int i = 0; i < vec->size; i++)
        free(vec->children[i]);

    free(vec->children);
    free(vec);
    *p_vec = NULL;

    return;
}

const struct vector_tree_node_lib VectorNodes = { .new = vtree_new, .size = vtree_size, .insert = vtree_insert, .erase = vtree_erase, .push_back = vtree_push_back, .push_front = vtree_push_front, .pop_back = vtree_pop_back, .pop_front = vtree_pop_front, .at = vtree_at, .print = vtree_print, .free = vtree_free };


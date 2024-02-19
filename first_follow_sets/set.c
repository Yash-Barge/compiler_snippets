#include "set.h"

struct set {
    enum terminals *term;
    int size;
    int capacity;
};

struct set *new_set(void) {
    struct set *st = malloc(sizeof(struct set));
    *st = (struct set) { .size = 0, .capacity = 1 };
    st->term = malloc(sizeof(enum terminals) * st->capacity);

    return st;
}

void grow_set(struct set *st) {
    st->term = realloc(st->term, sizeof(enum terminals) * st->capacity * 2);
    st->capacity *= 2;

    return;
} 

int set_size(struct set* st){
    assert(st != NULL);
    return st->size;
}

void set_insert(struct set *st, enum terminals term) {
    assert(st != NULL);

    if(st->size == st->capacity)
        grow_set(st);


    st->term[st->size] = term;
    st->size++;

    return;
}

void set_print(struct set *st) {
    assert(st != NULL);

    for(int i = 0; i < st->size; i++)
        printf("%d ", st->term[i]);
    printf("\n");

    return;
}

void set_free(struct set **p_set) {
    assert(p_set != NULL);
    assert(*p_set != NULL);

    struct set *st = *p_set;
    free(st->term);
    free(st);
    *p_set = NULL;

    return;
}

const struct set_lib Set = { .new = new_set, .size = set_size, .insert = set_insert, .print = set_print, .free = set_free };

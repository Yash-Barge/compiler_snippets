#include <string.h>

#include "set.h"

struct set {
    enum terminals *term;
    int size;
    int capacity;
};

/**
 * @brief Initializes a new set
 * 
 * @return struct set* The pointer to the newly allocated Set ADT
 */
struct set *new_set(void) {
    struct set *st = malloc(sizeof(struct set));
    *st = (struct set) { .size = 0, .capacity = 1 };
    st->term = malloc(sizeof(enum terminals) * st->capacity);

    return st;
}

/**
 * @brief Increases the size of the set
 * 
 * @param st The pointer to the set
 */
void grow_set(struct set *st) {
    st->term = realloc(st->term, sizeof(enum terminals) * st->capacity * 2);
    st->capacity *= 2;

    return;
} 

/**
 * @brief Return the size of the set
 * 
 * @param st The pointer to the set
 * @return int The size of the set
 */
int set_size(struct set* st){
    assert(st != NULL);
    return st->size;
}

/**
 * @brief Searches the set for the searchVal
 * 
 * @param st The pointer to the set
 * @param searchVal The value to be searched
 * @return int Return 1 if the element is found, and 0 if not
 */
int set_search(struct set *st, int searchVal){
    assert(st != NULL);
    for(int i=0; i<st->size; i++)
        if((int) st->term[i] == searchVal) return 1;
    return 0;
}

/**
 * @brief Insert a terminal into the set
 * 
 * @param st Pointer to the set
 * @param term The terminal that we want to add to the set
 */
void set_insert(struct set *st, enum terminals term) {
    assert(st != NULL);

    if(!set_search(st, term)){
        if(st->size == st->capacity)
            grow_set(st);


        st->term[st->size] = term;
        st->size++;
    }
    return;
}

/**
 * @brief Get the element at a particular index of the set
 * 
 * @param st The pointer of the set
 * @param i The index, whose element we want to find
 * @return int The terminal at the i'th index of the set
 */
int set_at(struct set *st, int i){
    assert(st != NULL);
    assert(i < st->size);
    assert(i >= 0);

    return st->term[i];
}

/**
 * @brief Print the elements of the set
 * 
 * @param st Pointer to the set
 */
void set_print(struct set *st) {
    assert(st != NULL);

    for(int i = 0; i < st->size; i++)
        printf("%d ", st->term[i]);
    printf("\n");

    return;
}

/**
 * @brief Free the memory allocated to the set
 * 
 * @param p_set Pointer to pointer of the set
 */
void set_free(struct set **p_set) {
    assert(p_set != NULL);
    assert(*p_set != NULL);

    struct set *st = *p_set;
    free(st->term);
    free(st);
    *p_set = NULL;

    return;
}

const struct set_lib Set = { .new = new_set, .size = set_size, .insert = set_insert, .at = set_at, .search = set_search, .print = set_print, .free = set_free };

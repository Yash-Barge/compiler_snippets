#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "vector.h"

struct vector_int {
    int *vector;
    int size;
    int capacity;
};

/**
 * @brief Initialise a new Vector of Integers
 * 
 * @return struct vector_int* A pointer to the vector
 */
struct vector_int *vint_new(void) {
    struct vector_int *vec = malloc(sizeof(*vec));
    *vec = (struct vector_int) { .size = 0, .capacity = 1 };
    vec->vector = malloc(sizeof(*vec->vector) * vec->capacity);

    return vec;
}

/**
 * @brief Used to dynamically increase the size of the vector
 * 
 * @param vec The pointer to the vector
 */
void vint_grow_vector(struct vector_int *vec) {
    vec->vector = realloc(vec->vector, sizeof(*vec->vector) * vec->capacity * 2);
    vec->capacity *= 2;

    return;
} 

/**
 * @brief Returns the size of the vector
 * 
 * @param vec The pointer to the vector
 * @return int The size of the vector
 */
int vint_size(struct vector_int *vec){
    assert(vec != NULL);
    return vec->size;
}

/**
 * @brief Insert an element into the vector
 * 
 * @param vec The pointer to the vector
 * @param index The index at which we want to insert
 * @param data The data that we want to add at index
 */
void vint_insert(struct vector_int *vec, int index, int data) {
    assert(vec != NULL);
    assert(index >= 0 && index <= vec->size);

    if(vec->size == vec->capacity)
        vint_grow_vector(vec);

    memmove(vec->vector + index + 1, vec->vector + index, sizeof(*vec->vector) * (vec->size++ - index));

    vec->vector[index] = data;

    return;
}

/**
 * @brief Searches for an element in the vector, and returns it's index
 * 
 * @param vec The pointer to the vector
 * @param val The value which we want to search for
 * @return int The index of the first occurrence of that val. If that val does not exist in the vector, then return -1
 */
int vint_index_of(struct vector_int *vec, int val){
    assert(vec != NULL);
    for(int i = 0; i < vec->size; i++)
        if (vec->vector[i] == val)
            return i;
    return -1;
}

/**
 * @brief Erase the element at a particular index 
 * 
 * @param vec The pointer to the vector
 * @param index The index, whose element we want to delete
 * @return int The Value at that index
 */
int vint_erase(struct vector_int *vec, int index) {
    assert(vec != NULL);
    assert(index >= 0 && index < vec->size);

    int val = vec->vector[index];

    memmove(vec->vector + index, vec->vector + index + 1, sizeof(*vec->vector) * (--vec->size - index));

    return val;
}

/**
 * @brief Erase the element at the last index 
 * 
 * @param vec The pointer to the vector
 * @return int The element at the last index
 */
int vint_pop_back(struct vector_int *vec) {
    return vint_erase(vec, vec->size-1); 
}

/**
 * @brief Erase the element at the zero'th index
 * 
 * @param vec The pointer to the vector
 * @return int The element at the zero'th index
 */
int vint_pop_front(struct vector_int *vec) {
    return vint_erase(vec, 0); 
}

/**
 * @brief Add an element at the end of the vector
 * 
 * @param vec The pointer to the vector
 * @param val The value to be added at the end of the vector
 */
void vint_push_back(struct vector_int *vec, int val) {
    return vint_insert(vec, vec->size, val); 
}

/**
 * @brief Add an element at the beginning of the vector
 * 
 * @param vec The pointer to the vector
 * @param val The value to be added at the beginning of the vector
 */
void vint_push_front(struct vector_int *vec, int val) {
    return vint_insert(vec, 0, val); 
}

/**
 * @brief Returns the value at a particular index of the vector
 * 
 * @param vec The pointer to the vector
 * @param index The index 
 * @return int The value at the index we want to check
 */
int vint_at(struct vector_int *vec, int index) {
    return vec->vector[index];
}

/**
 * @brief Print the vector
 * 
 * @param vec The pointer to the vector
 */
void vint_print(struct vector_int *vec) {
    assert(vec != NULL);

    for(int i = 0; i < vec->size; i++)
        printf("%d ", vec->vector[i]);
    printf("\n");

    return;
}

/**
 * @brief Free the memory allocated to the vector
 * 
 * @param p_vec The pointer to the pointer to the vector
 */
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

struct vector_string {
    char **vector;
    int size;
    int capacity;
};


/**
 * @brief Initialise a new Vector of Strings
 * 
 * @return struct vector_string* A pointer to the vector
 */
struct vector_string *vstring_new(void) {
    struct vector_string *vec = malloc(sizeof(*vec));
    *vec = (struct vector_string) { .size = 0, .capacity = 1 };
    vec->vector = malloc(sizeof(*vec->vector) * vec->capacity);

    return vec;
}

/**
 * @brief Increase the size of the vector
 * 
 * @param vec The pointer of the vector
 */
void vstring_grow_vector(struct vector_string *vec) {
    vec->vector = realloc(vec->vector, sizeof(*vec->vector) * vec->capacity * 2);
    vec->capacity *= 2;

    return;
} 

/**
 * @brief Get the size of the vector
 * 
 * @param vec The pointer of the vector
 * @return int The size of the vector
 */
int vstring_size(struct vector_string *vec){
    assert(vec != NULL);
    return vec->size;
}

/**
 * @brief Insert an element into the vector at a particular index
 * 
 * @param vec The pointer of the vector
 * @param index The index where we want to add
 * @param data The item we want to add to the vector
 */
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

/**
 * @brief Erase an element at a particular index
 * 
 * @param vec The pointer of the vector
 * @param index The index of the element which we want to delete
 * @return char* The Value at index
 */
char *vstring_erase(struct vector_string *vec, int index) {
    assert(vec != NULL);
    assert(index >= 0 && index < vec->size);

    char *val = vec->vector[index];

    memmove(vec->vector + index, vec->vector + index + 1, sizeof(*vec->vector) * (--vec->size - index));

    return val;
}

/**
 * @brief Delete the element at the last index
 * 
 * @param vec The pointer of the vector
 * @return char* The element at the last index
 */
char *vstring_pop_back(struct vector_string *vec) {
    return vstring_erase(vec, vec->size-1); 
}

/**
 * @brief Delete the element at the zero'th index
 * 
 * @param vec The pointer of the vector
 * @return char* The element at the zero'th index
 */
char *vstring_pop_front(struct vector_string *vec) {
    return vstring_erase(vec, 0); 
}

/**
 * @brief Add an element at the end of the vector
 * 
 * @param vec The pointer of the vector
 * @param val The value which we want to add at the end of the vector
 */
void vstring_push_back(struct vector_string *vec, char *val) {
    return vstring_insert(vec, vec->size, val); 
}

/**
 * @brief Add an element to the beginning of the vector
 * 
 * @param vec The pointer of the vector
 * @param val The value which we want to add at the beginning of the vector
 */
void vstring_push_front(struct vector_string *vec, char *val) {
    return vstring_insert(vec, 0, val); 
}

/**
 * @brief Get the string at a particular index
 * 
 * @param vec The pointer of the vector
 * @param index The index whose element we want to find
 * @return const char* The element at that index
 */
const char *vstring_at(struct vector_string *vec, int index) {
    return vec->vector[index];
}

/**
 * @brief Print all the elements of the vector
 * 
 * @param vec The pointer of the vector
 */
void vstring_print(struct vector_string *vec) {
    assert(vec != NULL);

    for(int i = 0; i < vec->size; i++)
        printf("%s\n", vec->vector[i]);
    printf("\n");

    return;
}

/**
 * @brief Free the memory allocated to the vector
 * 
 * @param p_vec A pointer to the pointer of the vector
 */
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

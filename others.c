/*
    GROUP NUMBER - 27
    Shubham Gupta - 2021A7PS0468P
    Yash Sangram Barge - 2021A7PS0006P
    Venkatavihan Devaki - 2021A7PS0429P
    Priyank Shethia - 2021A7PS0657P
    Aditya Khandelwal - 2021A7PS2422P
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "others.h"

#define NEW_ST_KEYWORD_ENTRY(_keyword, _tok) ((struct st_internal_keyword) { .keyword = _keyword, .tok = _tok })

static int lexer_error_count = 0;
static int parser_error_count = 0;

/**
 * @brief Used to print a case of errors
 * 
 * @param fmt Error Message
 * @param ... 
 */
void error(const char *fmt, ...) {
    char *prefix = "\033[1;31merror: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    return;
}

/**
 * @brief Used to print a case of Lexical Errors
 * 
 * @param fmt Error Message
 * @param ... 
 */
void lexer_error(const char *fmt, ...) {
    lexer_error_count++;
    
    char *prefix = "\033[1;31merror: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    return;
}

/**
 * @brief Used to print a case of Parser Errors
 * 
 * @param fmt Error Message
 * @param ... 
 */
void parser_error(const char *fmt, ...) {
    parser_error_count++;

    char *prefix = "\033[1;31merror: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    return;
}

/**
 * @brief Get the lexer error count object
 * 
 * @return int Number of Errors
 */
int get_lexer_error_count(void) {
    return lexer_error_count;
}

/**
 * @brief Get the parser error count object
 * 
 * @return int Number of Parser Errors
 */
int get_parser_error_count(void) {
    return parser_error_count;
}

/**
 * @brief Reset the error count for Lexer and Parser
 * 
 */
void reset_error_count(void) {
    lexer_error_count = 0;
    parser_error_count = 0;
    return;
}




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





//defining stack_node in stack
struct stack_node {
    int data;
    struct stack_node *next;
};

/**
 * @brief creates new stack_node
 * 
 * @param data 
 * @return struct stack_node* 
 */
struct stack_node *new_node(int data) {
    struct stack_node *n = malloc(sizeof(struct stack_node));
    *n = (struct stack_node) { .data = data, .next = NULL };
    return n;
}

//defines stack 
struct stack {
    struct stack_node *top;
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
    struct stack_node *n = new_node(data);

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

    struct stack_node *temp = s->top;
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






struct st_internal_keyword {
    char *keyword;
    enum terminals tok;
};

// Lexicographically sorted

struct st_internal_keyword token_list[] = {
    NEW_ST_KEYWORD_ENTRY("_main", TK_MAIN),
    NEW_ST_KEYWORD_ENTRY("as", TK_AS),
    NEW_ST_KEYWORD_ENTRY("call", TK_CALL),
    NEW_ST_KEYWORD_ENTRY("definetype", TK_DEFINETYPE),
    NEW_ST_KEYWORD_ENTRY("else", TK_ELSE),
    NEW_ST_KEYWORD_ENTRY("end", TK_END),
    NEW_ST_KEYWORD_ENTRY("endif", TK_ENDIF),
    NEW_ST_KEYWORD_ENTRY("endrecord", TK_ENDRECORD),
    NEW_ST_KEYWORD_ENTRY("endunion", TK_ENDUNION),
    NEW_ST_KEYWORD_ENTRY("endwhile", TK_ENDWHILE),
    NEW_ST_KEYWORD_ENTRY("global", TK_GLOBAL),
    NEW_ST_KEYWORD_ENTRY("if", TK_IF),
    NEW_ST_KEYWORD_ENTRY("input", TK_INPUT),
    NEW_ST_KEYWORD_ENTRY("int", TK_INT),
    NEW_ST_KEYWORD_ENTRY("list", TK_LIST),
    NEW_ST_KEYWORD_ENTRY("output", TK_OUTPUT),
    NEW_ST_KEYWORD_ENTRY("parameter", TK_PARAMETER),
    NEW_ST_KEYWORD_ENTRY("parameters", TK_PARAMETERS),
    NEW_ST_KEYWORD_ENTRY("read", TK_READ),
    NEW_ST_KEYWORD_ENTRY("real", TK_REAL),
    NEW_ST_KEYWORD_ENTRY("record", TK_RECORD),
    NEW_ST_KEYWORD_ENTRY("return", TK_RETURN),
    NEW_ST_KEYWORD_ENTRY("then", TK_THEN),
    NEW_ST_KEYWORD_ENTRY("type", TK_TYPE),
    NEW_ST_KEYWORD_ENTRY("union", TK_UNION),
    NEW_ST_KEYWORD_ENTRY("while", TK_WHILE),
    NEW_ST_KEYWORD_ENTRY("with", TK_WITH),
    NEW_ST_KEYWORD_ENTRY("write", TK_WRITE)
};

// Start point for each linked list of the Symbol Table
struct st_head {
    struct st_node* first_node;
};

// Nodes of the Linked list
struct st_node {
    struct st_data val;
    struct st_node* next;
};

// Array of st_head pointers, our main data structure that must be accessed for all further operations
struct symbol_table {
    struct st_head** map;
    int size;
    int capacity;
};

struct symbol_table* symbol_table_new(int initial_capacity);
struct st_data *symbol_table_insert(struct symbol_table* hm, char* key, enum terminals tk);
struct st_data *symbol_table_search(struct symbol_table* hm, char* key, enum terminals tk);
void symbol_table_erase(struct symbol_table* hm, char* key);
void symbol_table_free(struct symbol_table **p_hm);

/**
 * @brief Given a string (or Lexeme) and token, it returns a st_node with these fields
 * 
 * @param tok Token index
 * @param val The Lexeme
 * @return Pointer to the st_node for that Lexeme.
 */
struct st_node* new_node_symbol_table_with_tok(enum terminals tok, char* val) {
    struct st_node* n = malloc(sizeof(struct st_node));

    if (tok == TK_NUM) {
        n->val.stringLexeme = malloc((strlen(val) + 1) * sizeof(char));
        strcpy(n->val.stringLexeme, val);
        long long temp = atoi(val);
        n->val.lexeme.intVal= temp;
        n->val.token_type = TK_NUM;
    } else if (tok == TK_RNUM) {
        n->val.stringLexeme = malloc((strlen(val) + 1) * sizeof(char));
        strcpy(n->val.stringLexeme, val);
        double temp = atof(val);
        n->val.lexeme.floatVal = temp;
        n->val.token_type = TK_RNUM;
    } else {
        n->val.stringLexeme = malloc(sizeof(char) * (strlen(val) + 1));
        strcpy(n->val.stringLexeme, val);
        n->val.token_type = tok;
    }
    n->next = NULL;
    return n;
}

/**
 * @brief Returns a st_head pointer for the Linked List in the Symbol Table
 * 
 * @return st_head pointer, that initially points to NULL
 */
struct st_head* new_head_symbol_table(void) {
    struct st_head* h = malloc(sizeof(struct st_head));
    h->first_node = NULL;
    return h;
}

/**
 * @brief Given the st_head pointer and Lexeme, it inserts a st_node for the Lexeme into the linked list pointed to by st_head.
 * 
 * @param h The st_head pointer
 * @param val The Lexeme
 */
void ll_insert(struct st_head* h, char* val, enum terminals tk) {
    assert(h != NULL);

    struct st_node* n = new_node_symbol_table_with_tok(tk, val);

    assert(n != NULL);

    n->next = h->first_node;
    h->first_node = n;

    return;
}

/**
 * @brief Given the st_head pointer to a Linked List, free all the memory it uses dynamically.
 * 
 * @param h st_head pointer to the linked list.
 */
void ll_free(struct st_head* h) {
    assert(h != NULL);

    if (h->first_node == NULL) {
        free(h);
        return;
    }

    struct st_node* temp = h->first_node;
    struct st_node* prev = h->first_node;

    while (temp != NULL) {
        prev = temp;
        temp = temp->next;

        if(prev->val.token_type == TK_NUM || prev->val.token_type == TK_RNUM) {
            free(prev->val.stringLexeme);
            free(prev);
        }

        else {
            free(prev->val.stringLexeme);
            free(prev);
        }
    }

    free(h);
    return;
}

/**
 * @brief The Hash function used to hash into the Symbol Table
 * 
 * @param tok the token
 * @param key The Lexeme
 * @param capacity The current capacity of the array of the Symbol Table
 * @return index for the symbol table
 */
int hash_function(enum terminals tok, char* key, int capacity) {
    if (tok == TK_NUM || tok == TK_RNUM)
        return (1LL * atoi(key) * 23) % capacity;

    return (1LL * strlen(key) * 23) % capacity;
}

/**
 * @brief Used to create a new Symbol Table
 * 
 * @param initial_capacity Initial size of the Symbol Table (May Change)
 * @return Pointer to the symbol table
 */
struct symbol_table* symbol_table_new(int initial_capacity) {
    struct symbol_table* st = malloc(sizeof(struct symbol_table));
    st->map = malloc(sizeof(struct st_head*) * initial_capacity);
    for(int i=0; i < initial_capacity; i++){
        st->map[i] = new_head_symbol_table();
    }
    st->capacity = initial_capacity;
    st->size = 0;
    return st;
}

/**
 * @brief Used to insert a lexeme into the Symbol Table
 * 
 * @param hm Symbol Table pointer
 * @param key The Lexeme
 */
struct st_data *symbol_table_insert(struct symbol_table* hm, char* key, enum terminals tk) {
    if (symbol_table_search(hm, key, tk))
        return symbol_table_search(hm, key, tk);

    int index = hash_function(tk, key, hm->capacity);
    
    ll_insert(hm->map[index], key, tk);
    hm->size++;

    if (hm->size == hm->capacity) {
        struct symbol_table* new_hm = symbol_table_new(hm->capacity * 2);

        for (int i = 0; i < hm->capacity; i++) {
            struct st_node* temp = hm->map[i]->first_node;

            while(temp != NULL) {
                if (temp->val.token_type == TK_NUM) {
                    // int len = snprintf(NULL, 0, "%lld",  temp->val.lexeme.intVal);
                    // char buffer[len+1];
                    // snprintf(buffer, len + 1, "%lld",  temp->val.lexeme.intVal);
                    symbol_table_insert(new_hm, temp->val.stringLexeme, TK_NUM);
                } else if (temp->val.token_type == TK_RNUM) {
                    // int len = snprintf(NULL, 0, "%lf",  temp->val.lexeme.floatVal);
                    // char buffer[len+1];
                    // snprintf(buffer, len + 1, "%lf",  temp->val.lexeme.floatVal);
                    symbol_table_insert(new_hm, temp->val.stringLexeme, TK_RNUM);
                } else {
                    symbol_table_insert(new_hm, temp->val.stringLexeme, temp->val.token_type);
                }

                temp = temp->next;
            }
        }

        // swaps memory of old symbol table and new symbol table, then frees the old one
        struct symbol_table st_temp;
        memcpy(&st_temp, hm, sizeof(st_temp));
        memcpy(hm, new_hm, sizeof(st_temp));
        memcpy(new_hm, &st_temp, sizeof(st_temp));
        symbol_table_free(&new_hm);
    }

    return symbol_table_search(hm, key, tk);
}

/**
 * @brief Used to search for an element in a Symbol Table
 * 
 * @param hm Symbol Table
 * @param key The Lexeme
 * @param tk Token type (as enum)
 * @return Return pointer to symbol table entry if it exists, else NULL
 */
struct st_data *symbol_table_search(struct symbol_table* hm, char* key, enum terminals tk) {
    assert(hm != NULL);

    int index = hash_function(tk, key, hm->capacity);

    if (hm->map[index]->first_node == NULL)
        return NULL;

    struct st_node* temp = hm->map[index]->first_node;

    while (temp != NULL) {
        if (temp->val.token_type == tk) {
            if (!(strcmp(temp->val.stringLexeme, key)))
                return &(temp->val);
        }

        temp = temp->next;
    }

    return NULL;
}

/**
 * @brief Used to search for a keyword in a Symbol Table
 * 
 * @param hm Symbol Table
 * @param key The Lexeme/keyword
 * @return Return pointer to symbol table entry if it exists, else NULL
 */
struct st_data *symbol_table_search_keyword(struct symbol_table* hm, char* key) {
    assert(hm != NULL);

    // TODO: replace with binary search?
    for (unsigned long i = 0; i < sizeof(token_list) / sizeof(token_list[0]); i++)
        if (!strcmp(token_list[i].keyword, key))
            return symbol_table_search(hm, key, token_list[i].tok);

    return NULL;
}

/**
 * @brief Initialise the Symbol Table with all the Keywords
 * 
 * @return Initialised Symbol Table
 */
struct symbol_table* symbol_table_init(void) {
    struct symbol_table* st = symbol_table_new(50);

    for (unsigned long i = 0; i < sizeof(token_list) / sizeof(token_list[0]); i++)
        symbol_table_insert(st, token_list[i].keyword, token_list[i].tok);

    return st;
}

/**
 * @brief Used to free all the memory dynamically stored for a Symbol Table
 * 
 * @param p_hm Pointer to the Symbol Table Pointer
 */
void symbol_table_free(struct symbol_table **p_hm) {
    assert(p_hm != NULL);
    assert(*p_hm != NULL);

    struct symbol_table *hm = *p_hm;

    for(int i = 0; i < hm->capacity; i++)
        ll_free(hm->map[i]);

    free(hm->map);
    free(hm);

    *p_hm = NULL;

    return;
}

const struct symbol_table_lib SymbolTable = { .new = symbol_table_new, .insert = symbol_table_insert, .search = symbol_table_search, .search_keyword = symbol_table_search_keyword, .init = symbol_table_init, .free  = symbol_table_free };






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

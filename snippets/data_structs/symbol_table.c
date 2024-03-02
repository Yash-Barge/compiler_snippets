#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "symbol_table.h"
#include "../lexer_test/regex/regex.h"

// All representations of the tokens (Not identifiers) in Lexical order
char *const token_list[] = {
    "!=",
    "&&&",
    "(",
    ")",
    "*",
    "+",
    ",",
    "-",
    ".",
    "/",
    ":",
    ";",
    "<",
    "<---",
    "<=",
    "==",
    ">",
    ">=",
    "@@@",
    "[",
    "]",
    "_main",
    "as",
    "call",
    "definetype",
    "else",
    "end",
    "endif",
    "endrecord",
    "endunion",
    "endwhile",
    "global",
    "if",
    "input",
    "int",
    "list",
    "output",
    "parameter",
    "parameters",
    "read",
    "real",
    "record",
    "return",
    "then",
    "type",
    "union",
    "while",
    "with",
    "write",
    "~"
};


// Start point for each linked list of the Symbol Table
struct head {
    struct node* first_node;
};

typedef union {
    char* lexeme;
    long long intVal;
    double floatVal;
} dataElements;

// Data of the lexeme, which includes the lexeme's name and the type of token
struct data {
    dataElements lexeme;
    ENUM_TOKENS token_type;
};

// Nodes of the Linked list
struct node {
    struct data val;
    struct node* next;
};

// Array of head pointers, our main data structure that must be accessed for all further operations
struct symbol_table {
    struct head** map;
    int size;
    int capacity;
};

struct symbol_table* symbol_table_new(int size);
void symbol_table_insert(struct symbol_table** hm, char* key, ENUM_TOKENS tk);
int symbol_table_search(struct symbol_table* hm, char* key, ENUM_TOKENS tk);
void symbol_table_erase(struct symbol_table* hm, char* key);
void symbol_table_free(struct symbol_table **p_hm);


/**
 * @brief Given a string (or Lexeme) and token, it returns a node with these fields
 * 
 * @param tok Token index
 * @param val The Lexeme
 * @return Pointer to the node for that Lexeme.
 */
struct node* new_node_symbol_table_with_tok(ENUM_TOKENS tok, char* val){
    struct node* n = malloc(sizeof(struct node));

    if(tok == TK_NUM) {
        long long temp = atoi(val);
        n->val.lexeme.intVal= temp;
        n->val.token_type = TK_NUM;
    }
    else if(tok == TK_RNUM) {
        double temp = atof(val);
        n->val.lexeme.floatVal = temp;
        n->val.token_type = TK_RNUM;
    }
    else {
        n->val.lexeme.lexeme = malloc(sizeof(char) * (strlen(val) + 1));
        strcpy(n->val.lexeme.lexeme, val);
        n->val.token_type = tok;
    }

    n->next = NULL;
    return n;
}


/**
 * @brief Returns a head pointer for the Linked List in the Symbol Table
 * 
 * @return head pointer, that initially points to NULL
 */
struct head* new_head_symbol_table(void){
    struct head* h = malloc(sizeof(struct head));
    h->first_node = NULL;
    return h;
}


/**
 * @brief Given the head pointer and Lexeme, it inserts a node for the Lexeme into the linked list pointed to by head.
 * 
 * @param h The head pointer
 * @param val The Lexeme
 */
void ll_insert(struct head* h, char* val, ENUM_TOKENS tk){
    assert(h != NULL);
    struct node* n = new_node_symbol_table_with_tok(tk, val);
    if(n == NULL){
        printf("Invalid Token : %s\n", val);
        return;
    }
    n->next = h->first_node;
    h->first_node = n;
    return;
}



/**
 * @brief Given the head pointer to a Linked List, free all the memory it uses dynamically.
 * 
 * @param h head pointer to the linked list.
 */
void ll_free(struct head* h){
    assert(h != NULL);
    if(h->first_node == NULL) {
        free(h);
        return;
    }
    struct node* temp = h->first_node;
    struct node* prev = h->first_node;
    while(temp != NULL){
        prev = temp;
        temp = temp->next;
        if(prev->val.token_type == TK_NUM || prev->val.token_type == TK_RNUM) free(prev);
        else {
            free(prev->val.lexeme.lexeme);
            free(prev);
        }
    }
    free(h);
}



/**
 * @brief The Hash function used to hash into the Symbol Table
 * 
 * @param tok the token
 * @param key The Lexeme
 * @param size The current size of the array of the Symbol Table
 * @return index for the symbol table
 */
int hash_function(ENUM_TOKENS tok, char* key, int size){
    if(tok == TK_NUM || tok == TK_RNUM) {
        return (1LL * atoi(key) * 23) % size;
    }
    return (1LL * strlen(key) * 23) % size;
}

/**
 * @brief Used to create a new Symbol Table
 * 
 * @param size Initial size of the Symbol Table (May Change)
 * @return Pointer to the symbol table
 */
struct symbol_table* symbol_table_new(int size){
    struct symbol_table* st = malloc(sizeof(struct symbol_table));
    st->map = malloc(sizeof(struct head*) * size);
    for(int i=0; i<size; i++){
        st->map[i] = new_head_symbol_table();
    }
    st->size = size;
    return st;
}

/**
 * @brief Used to insert a lexeme into the Symbol Table
 * 
 * @param p_hm Pointer to the Symbol Table pointer
 * @param key The Lexeme
 */
void symbol_table_insert(struct symbol_table** p_hm, char* key, ENUM_TOKENS tk){ // Double pointer as we may need to resize it
    assert(p_hm != NULL);
    assert(*p_hm != NULL);

    struct symbol_table *hm = *p_hm;

    if (symbol_table_search(hm, key, tk))
        return;

    int index = hash_function(tk, key, hm->size);
    
    ll_insert(hm->map[index], key, tk);
    hm->capacity++;

    if(hm->capacity == hm->size){
        struct symbol_table* new_hm = symbol_table_new(hm->size * 2);
        for(int i=0; i<hm->size; i++){
            struct node* temp = hm->map[i]->first_node;
            while(temp != NULL) {
                if(temp->val.token_type == TK_NUM){
                    int len = snprintf(NULL, 0, "%d",  temp->val.lexeme.intVal);
                    char buffer[len+1];
                    snprintf(buffer, len + 1, "%d",  temp->val.lexeme.intVal);
                    symbol_table_insert(&new_hm, buffer, TK_NUM);
                }
                else if(temp->val.token_type == TK_RNUM) {
                    int len = snprintf(NULL, 0, "%f",  temp->val.lexeme.floatVal);
                    char buffer[len+1];
                    snprintf(buffer, len + 1, "%f",  temp->val.lexeme.floatVal);
                    symbol_table_insert(&new_hm, buffer, TK_RNUM);
                }
                else{
                    symbol_table_insert(&new_hm, temp->val.lexeme.lexeme, temp->val.token_type);
                }
                temp = temp->next;
            }
        }
        symbol_table_free(p_hm);
        *p_hm = new_hm;
    }
    return;
}

/**
 * @brief Used to search for an element in a Symbol Table
 * 
 * @param hm Pointer to the Symbol Table
 * @param key The Lexeme
 * @return Return 0 if it isn't present in the Symbol Table, else return 1
 */
int symbol_table_search(struct symbol_table* hm, char* key, ENUM_TOKENS tk){
    assert(hm != NULL);
    int index = hash_function(tk, key, hm->size);
    if(hm->map[index]->first_node == NULL) return 0;
    struct node* temp = hm->map[index]->first_node;
    while(temp != NULL){
        if(temp->val.token_type == tk && tk == TK_NUM && (temp->val.lexeme.intVal == atoi(key))) return 1;
        else if(temp->val.token_type == tk && tk == TK_RNUM && (temp->val.lexeme.floatVal == atof(key))) return 1;
        else if(temp->val.token_type == tk && !strcmp(temp->val.lexeme.lexeme, key)) return 1;
        temp = temp->next;
    }
    return 0;
}

/**
 * @brief Initialise the Symbol Table with all the Keywords
 * 
 * @return Initialised Symbol Table
 */
struct symbol_table* symbol_table_init(void){
    struct symbol_table* st = symbol_table_new(50);
    for(int i=0; i<50; i++){
        symbol_table_insert(&st, token_list[i], i);
    }
    return st;
}

/**
 * @brief Used to free all the memory dynamically stored for a Symbol Table
 * 
 * @param p_hm Pointer to the Symbol Table Pointer
 */
void symbol_table_free(struct symbol_table **p_hm){
    assert(p_hm != NULL);
    assert(*p_hm != NULL);

    struct symbol_table *hm = *p_hm;

    int symbol_table_size = hm->size;
    for(int i=0; i<symbol_table_size; i++){
        ll_free(hm->map[i]);
    }
    free(hm->map);
    free(hm);

    *p_hm = NULL;

    return;
}

const struct symbol_table_lib SymbolTable = {.new = symbol_table_new, .insert = symbol_table_insert, .search = symbol_table_search, .init = symbol_table_init, .free  = symbol_table_free};
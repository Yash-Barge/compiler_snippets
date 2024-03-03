#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "symbol_table.h"

#define NEW_ST_KEYWORD_ENTRY(_keyword, _tok) ((struct st_internal_keyword) { .keyword = _keyword, .tok = _tok })

// All representations of the tokens (Not identifiers) in Lexical order
// char *const token_list[] = {
//     "!=",
//     "&&&",
//     "(",
//     ")",
//     "*",
//     "+",
//     ",",
//     "-",
//     ".",
//     "/",
//     ":",
//     ";",
//     "<",
//     "<---",
//     "<=",
//     "==",
//     ">",
//     ">=",
//     "@@@",
//     "[",
//     "]",
//     "_main",
//     "as",
//     "call",
//     "definetype",
//     "else",
//     "end",
//     "endif",
//     "endrecord",
//     "endunion",
//     "endwhile",
//     "global",
//     "if",
//     "input",
//     "int",
//     "list",
//     "output",
//     "parameter",
//     "parameters",
//     "read",
//     "real",
//     "record",
//     "return",
//     "then",
//     "type",
//     "union",
//     "while",
//     "with",
//     "write",
//     "~"
// };

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
        long long temp = atoi(val);
        n->val.lexeme.intVal= temp;
        n->val.token_type = TK_NUM;
    } else if (tok == TK_RNUM) {
        double temp = atof(val);
        n->val.lexeme.floatVal = temp;
        n->val.token_type = TK_RNUM;
    } else {
        n->val.lexeme.lexeme = malloc(sizeof(char) * (strlen(val) + 1));
        strcpy(n->val.lexeme.lexeme, val);
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

    if (n == NULL) {
        printf("Invalid Token : %s\n", val);
        return;
    }

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

        if(prev->val.token_type == TK_NUM || prev->val.token_type == TK_RNUM)
            free(prev);
        else {
            free(prev->val.lexeme.lexeme);
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
                    int len = snprintf(NULL, 0, "%lld",  temp->val.lexeme.intVal);
                    char buffer[len+1];
                    snprintf(buffer, len + 1, "%lld",  temp->val.lexeme.intVal);
                    symbol_table_insert(new_hm, buffer, TK_NUM);
                } else if (temp->val.token_type == TK_RNUM) {
                    int len = snprintf(NULL, 0, "%lf",  temp->val.lexeme.floatVal);
                    char buffer[len+1];
                    snprintf(buffer, len + 1, "%lf",  temp->val.lexeme.floatVal);
                    symbol_table_insert(new_hm, buffer, TK_RNUM);
                } else {
                    symbol_table_insert(new_hm, temp->val.lexeme.lexeme, temp->val.token_type);
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
            if (tk == TK_NUM) {
                if (temp->val.lexeme.intVal == atoll(key))
                    return &(temp->val);
            } else if (tk == TK_RNUM) {
                if (temp->val.lexeme.floatVal == atof(key))
                    return &(temp->val);
            } else if (!strcmp(temp->val.lexeme.lexeme, key))
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "symbol_table.h"
#include "../lexer_test/regex/regex.h"

// All Tokens (Except Identifiers)
enum ENUM_TOKENS {
    TOKEN_INVALID = -1,
    TK_NE,
    TK_AND,
    TK_OP,
    TK_CL,
    TK_MUL,
    TK_PLUS,
    TK_COMMA,
    TK_MINUS,
    TK_DOT,
    TK_DIV,
    TK_COLON,
    TK_SEMI,
    TK_LT,
    TK_ASSIGNOP,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_OR,
    TK_SQL,
    TK_SQR,
    TK_MAIN,
    TK_AS,
    TK_CALL,
    TK_DEFINETYPE,
    TK_ELSE,
    TK_END,
    TK_ENDIF,
    TK_ENDRECORD,
    TK_ENDUNION,
    TK_ENDWHILE,
    TK_GLOBAL,
    TK_IF,
    TK_INPUT,
    TK_INT,
    TK_LIST,
    TK_OUTPUT,
    TK_PARAMETER,
    TK_PARAMETERS,
    TK_READ,
    TK_REAL,
    TK_RECORD,
    TK_RETURN,
    TK_THEN,
    TK_TYPE,
    TK_UNION,
    TK_WHILE,
    TK_WITH,
    TK_WRITE,
    TK_NOT,
    TOKEN_COUNT // Just to show count of tokens
};

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

// All Identifiers
enum ENUM_IDENTIFIERS {
    TK_INVALID = -1,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_FUNID,
    TK_RUID,
    TK_RNUM,
    TK_COUNT
};

// All regular expressions corresponding to the identifiers
char *regular_expressions[] = {
    "[alp][alp]*",
    "[b-d][2-7][b-d]*[2-7]*",
    "[0-9][0-9]*",
    "[_-_][alp][alp]*[0-9]*",
    "[#-#][alp][alp]",
    "[0-9][0-9]*[.-.][0-9][0-9]",
    "[0-9][0-9]*[.-.][0-9][0-9][E-E][+-+][0-9][0-9]",
    "[0-9][0-9]*[.-.][0-9][0-9][E-E][---][0-9][0-9]",
    "[0-9][0-9]*[.-.][0-9][0-9][E-E][0-9][0-9]",
};

// Start point for each linked list of the Symbol Table
struct head {
    struct node* first_node;
};

// Union for if the token is an identifier or not
typedef union {
    enum ENUM_TOKENS tok;
    enum ENUM_IDENTIFIERS id;
} ENUM_TYPE;

// Data of the lexeme, which includes the lexeme's name and the type of token
struct data {
    char* lexeme;
    ENUM_TYPE* token_type;
};

// Nodes of the Linked list
struct node {
    struct data* val;
    struct node* next;
};

// Array of head pointers, our main data structure that must be accessed for all further operations
struct symbol_table {
    struct head** map;
    int size;
    int capacity;
};

struct symbol_table* symbol_table_new(int size);
void symbol_table_insert(struct symbol_table** hm, char* key);
int symbol_table_search(struct symbol_table* hm, char* key);
void symbol_table_erase(struct symbol_table* hm, char* key);
void symbol_table_free(struct symbol_table **p_hm);

/**
 * @brief Returns the identifier ID by checking all the regex's and returns the index that satisfies it, else returns -1. 
 * 
 *
 * @param str String for which we want to find which class of identifier it belongs to
 * @return Identifier index from the ENUM_IDENTIFIERS
 */
enum ENUM_IDENTIFIERS token_from_string_id(char* str) {
    for(int i=0; i<5; i++){
        if(check(regular_expressions[i], str)) return i;
    }
    for(int i=5; i<9; i++){
        if(check(regular_expressions[i], str)) return 5;
    }
    return TK_INVALID;
}

/**
 * @brief Returns the identifier ID by checking tokens and returns the index that satisfies it, else returns -1. 
 * 
 *
 * @param str String for which we want to find which class of identifier it belongs to
 * @return Token index from the ENUM_TOKENS
 */
enum ENUM_TOKENS token_from_string_tok(char* str) {
    int bottom = 0;
    int top = TOKEN_COUNT - 1;
    int valid_token = 0;

    while (top >= bottom) {
        int comparison = strcmp(str, token_list[(top + bottom) / 2]);

        if (!comparison) {
            valid_token = 1;
            break;
        } else if (comparison > 0)
            bottom = (top + bottom) / 2 + 1;
        else
            top = (top + bottom) / 2 - 1;
    }

    if (!valid_token)
        return TOKEN_INVALID;

    return (top + bottom) / 2;
}

/**
 * @brief Given a string (or Lexeme) and token, it returns a node with these fields
 * 
 * @param tok Token index
 * @param val The Lexeme
 * @return Pointer to the node for that Lexeme.
 */
struct node* new_node_symbol_table_with_tok(int tok, char* val){
    struct node* n = malloc(sizeof(struct node));
    n->val = malloc(sizeof(struct data));
    n->val->lexeme = malloc((strlen(val) + 1) * sizeof(char));
    n->val->token_type = malloc(sizeof(ENUM_TYPE));
    if(token_from_string_tok(val) == -1){
        if(token_from_string_id(val) == -1){
            return NULL; // Neither a Identifier token or of any other type.
        }
        else{
            n->val->token_type->id = tok;
        }
    }
    else{
        n->val->token_type->tok = tok;
    }
    strcpy(n->val->lexeme, val);
    n->next = NULL;
    return n;
}

/**
 * @brief Given a string (or Lexeme), it returns a node, with the token type found from ENUM_IDENTIFIERS or ENUM_TOKENS
 * 
 * @param val The Lexeme
 * @return Pointer to the node for that Lexeme.
 */
struct node* new_node_symbol_table_without_tok(char* val){
    struct node* n = malloc(sizeof(struct node));
    n->val = malloc(sizeof(struct data));
    n->val->lexeme = malloc((strlen(val) + 1) * sizeof(char));
    n->val->token_type = malloc(sizeof(ENUM_TYPE));

    int tok_val = token_from_string_tok(val);
    int id_val = token_from_string_id(val);


    if(tok_val == -1){
        if(id_val == -1){
            return NULL; // Neither a Identifier token or of any other type.
        }
        else{
            n->val->token_type->id = id_val;
        }
    }
    else{
        n->val->token_type->tok = tok_val;
    }

    strcpy(n->val->lexeme, val);
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
void ll_insert(struct head* h, char* val){
    assert(h != NULL);
    struct node* n = new_node_symbol_table_without_tok(val);
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
        free(prev->val->lexeme);
        free(prev->val);
        free(prev);
    }
    free(h);
}

/**
 * @brief The Hash function used to hash into the Symbol Table
 * 
 * @param key The Lexeme
 * @param size The current size of the array of the Symbol Table
 * @return index for the symbol table
 */
int hash_function(char* key, int size){
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
void symbol_table_insert(struct symbol_table** p_hm, char* key){ // Double pointer as we may need to resize it
    assert(p_hm != NULL);
    assert(*p_hm != NULL);

    struct symbol_table *hm = *p_hm;

    if (symbol_table_search(hm, key))
        return;

    int index = hash_function(key, hm->size);
    ll_insert(hm->map[index], key);
    hm->capacity++;
    if(hm->capacity == hm->size){
        struct symbol_table* new_hm = symbol_table_new(hm->size * 2);
        for(int i=0; i<hm->size; i++){
            struct node* temp = hm->map[i]->first_node;
            while(temp != NULL) {
                symbol_table_insert(&new_hm, temp->val->lexeme);
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
int symbol_table_search(struct symbol_table* hm, char* key){
    assert(hm != NULL);
    int index = hash_function(key, hm->size);
    if(hm->map[index]->first_node == NULL) return 0;
    struct node* temp = hm->map[index]->first_node;
    while(temp != NULL){
        if(!strcmp(key, temp->val->lexeme)) return 1;
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
        symbol_table_insert(&st, token_list[i]);
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
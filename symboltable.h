#ifndef SYMTAB
#define SYMTAB
#include "enums.h"
struct row{
    char name[50];
    enum token_names token_type;
};

typedef struct node* Node;
struct node{
    struct row* entry;
    Node next;
};

typedef struct table* Table;
struct table{
    int count;
    Node head;
};

Table createtable();
void addrow(char* lexeme, enum token_names type, Table symboltable);
Node lookup(char* lexeme, Table symboltable);
void populate(Table symboltable);

#endif



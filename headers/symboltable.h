#ifndef SYMTAB
#define SYMTAB

#include "enums.h"

struct row{
    char name[50];
    enum terminals token_type;
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
void addrow(char* lexeme, enum terminals type, Table symboltable);
Node lookup(char* lexeme, Table symboltable);
void populate(Table symboltable);

#endif

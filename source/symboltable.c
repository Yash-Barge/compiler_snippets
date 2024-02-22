#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symboltable.h"

Table createtable(){
    Table symboltable;
    symboltable = malloc(sizeof(struct table));
    symboltable->count = 0;
    symboltable->head = NULL;
    return symboltable;
}

void addrow(char* lexeme, enum token_names type, Table symboltable){
    struct row* newrow;
    newrow = malloc(sizeof(struct row));
    strncpy(newrow->name, lexeme, sizeof(newrow->name));
    // is \0 necessary for above?
    newrow->token_type=type;

    Node newnode;
    newnode=(Node) malloc(sizeof(struct node));
    newnode->entry=newrow;
    newnode->next=NULL;

    if(symboltable->count==0){
        symboltable->head=newnode;
        newnode->next=NULL;
        symboltable->count++;
    } else {
        newnode->next=symboltable->head;
        symboltable->head=newnode;
        symboltable->count++;
    }

    return;
}

Node lookup(char* lexeme, Table symboltable){
    Node temp = symboltable->head;

    while (temp != NULL) {
        if (strcmp(lexeme, temp->entry->name) == 0)
            return temp;
        else
            temp=temp->next;
    }

    return NULL;
}

void populate(Table symboltable){
    char *names[] = {
        "with",
        "parameters",
        "end",
        "while",
        "union",
        "endunion",
        "definetype",
        "as",
        "type",
        "_main",
        "global",
        "parameter",
        "list",
        "input",
        "output",
        "int",
        "real",
        "endwhile",
        "if",
        "then",
        "endif",
        "read",
        "write",
        "return",
        "call",
        "record",
        "endrecord",
        "else"
    };

    enum token_names token_types[] = {
        TK_WITH,
        TK_PARAMETERS,
        TK_END,
        TK_WHILE,
        TK_UNION,
        TK_ENDUNION,
        TK_DEFINETYPE,
        TK_AS,
        TK_TYPE,
        TK_MAIN,
        TK_GLOBAL,
        TK_PARAMETER,
        TK_LIST,
        TK_INPUT,
        TK_OUTPUT,
        TK_INT,
        TK_REAL,
        TK_ENDWHILE,
        TK_IF,
        TK_THEN,
        TK_ENDIF,
        TK_READ,
        TK_WRITE,
        TK_RETURN,
        TK_CALL,
        TK_RECORD,
        TK_ENDRECORD,
        TK_ELSE
    };

    for(int i = 0; i < 28; i++)
        addrow(names[i], token_types[i], symboltable);
}
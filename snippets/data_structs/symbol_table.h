#pragma once

// All Tokens (Except Identifiers)
typedef enum ENUM_TOKENS {
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
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_FUNID,
    TK_RUID,
    TK_RNUM,
    TOKEN_COUNT // Just to show count of tokens
} ENUM_TOKENS;

struct symbol_table;

struct symbol_table_lib {
    struct symbol_table* (*const new)(int);
    void (*const insert)(struct symbol_table *, char*, ENUM_TOKENS);
    int (*const search)(struct symbol_table *, char*, ENUM_TOKENS);
    struct symbol_table* (*const init)();
    void (*const free)(struct symbol_table **);
};

extern const struct symbol_table_lib SymbolTable;

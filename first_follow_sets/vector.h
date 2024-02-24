#pragma once

struct vector_int;
struct vector_string;

struct vector_int_lib {
    struct vector_int *(*const new)(void);
    int (*const size)(struct vector_int *);
    void (*const insert)(struct vector_int *, int, int);
    int (*const search)(struct vector_int *, int);
    int (*const erase)(struct vector_int *, int);
    void (*const push_back)(struct vector_int *, int);
    int (*const pop_back)(struct vector_int *);
    void (*const push_front)(struct vector_int *, int);
    int (*const pop_front)(struct vector_int *);
    int (*const at)(struct vector_int *, int);
    void (*const print)(struct vector_int *);
    void (*const free)(struct vector_int **);
};

struct vector_string_lib {
    struct vector_string *(*const new)(void);
    int (*const size)(struct vector_string *);
    void (*const insert)(struct vector_string *, int, char *);
    char *(*const erase)(struct vector_string *, int);
    void (*const push_back)(struct vector_string *, char *);
    char *(*const pop_back)(struct vector_string *);
    void (*const push_front)(struct vector_string *, char *);
    char *(*const pop_front)(struct vector_string *);
    const char *(*const at)(struct vector_string *, int);
    void (*const print)(struct vector_string *);
    void (*const free)(struct vector_string **);
};

extern const struct vector_int_lib VectorInt;
extern const struct vector_string_lib VectorString;

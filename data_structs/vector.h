#pragma once

struct vector;

struct vector_lib {
    struct vector *(*const new)(void);
    int (*const size)(struct vector *);
    void (*const insert)(struct vector *, int, int);
    int (*const erase)(struct vector *, int);
    void (*const push_back)(struct vector *, int);
    int (*const pop_back)(struct vector *);
    void (*const push_front)(struct vector *, int);
    int (*const pop_front)(struct vector *);
    void (*const print)(struct vector *);
    void (*const sort)(struct vector *, int);
    void (*const free)(struct vector **);
};

extern const struct vector_lib Vector;

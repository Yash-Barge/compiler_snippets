#ifndef VECTOR_H
#define VECTOR_H

struct vector;

struct vector_lib {
    struct vector *(*const new)(void);
    void (*const insert)(struct vector *, int, int);
    int (*const erase)(struct vector *, int);
    void (*const push_back)(struct vector *, int);
    int (*const pop_back)(struct vector *);
    void (*const push_front)(struct vector *, int);
    int (*const pop_front)(struct vector *);
    void (*const print)(struct vector *);
    void (*const free)(struct vector *);
};

extern const struct vector_lib Vector;

#endif

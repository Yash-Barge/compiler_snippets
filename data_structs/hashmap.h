#ifndef HASHMAP_H
#define HASHMAP_H

struct hashmap;

struct hashmap_lib {
    struct hashmap *(*const new)(void);
    void (*const insert)(struct hashmap *, int);
    void (*const  erase)(struct hashmap *, int);
    int (*const search)(struct hashmap *, int);
    void (*const free)(struct hashmap **);
};

extern const struct hashmap_lib Hashmap;

#endif

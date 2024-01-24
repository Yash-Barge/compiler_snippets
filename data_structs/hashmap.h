#pragma once

struct hashmap;

struct hashmap_lib {
    struct hashmap *(*const new)(int);
    void (*const insert)(struct hashmap **, char*);
    void (*const  erase)(struct hashmap *, char*);
    int (*const search)(struct hashmap *, char*);
    void (*const free)(struct hashmap **);
};

extern const struct hashmap_lib Hashmap;

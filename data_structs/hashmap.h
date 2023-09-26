#pragma once

struct hashmap;

struct hashmap_lib {
    struct hashmap *(*const new)(int);
    void (*const insert)(struct hashmap *, int);
    void (*const  erase)(struct hashmap *, int);
    int (*const search)(struct hashmap *, int);
    void (*const free)(struct hashmap **);
};

extern const struct hashmap_lib Hashmap;

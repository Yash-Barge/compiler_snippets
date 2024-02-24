#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "enums.h"

struct set;

struct set_lib {
    struct set *(*const new)(void);
    int (*const size)(struct set *);
    void (*const insert)(struct set *, enum terminals);
    int (*const at)(struct set *, int);
    int (*const search)(struct set *, int);
    void (*const print)(struct set *);
    void (*const free)(struct set **);
};

extern const struct set_lib Set;

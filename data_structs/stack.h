#ifndef STACK_H
#define STACK_H

struct stack;

struct stack_lib {
    struct stack *(*const new)(void);
    void (*const push)(struct stack *, int);
    int (*const pop)(struct stack *);
    int (*const is_empty)(struct stack *);
    int (*const top)(struct stack *);
    void (*const free)(struct stack **);
};

extern const struct stack_lib Stack;

#endif

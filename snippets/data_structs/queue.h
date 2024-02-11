#pragma once

struct queue;

struct queue_lib {
    struct queue *(*const new)(void);
    void (*const push)(struct queue *, int);
    int (*const pop)(struct queue *);
    int (*const is_empty)(struct queue *);
    int (*const front)(struct queue *);
    void (*const free)(struct queue **);
};

extern const struct queue_lib Queue;

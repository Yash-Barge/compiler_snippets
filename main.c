#include <stdio.h>
#include <string.h>

#include "data_structs/stack.h"
#include "data_structs/vector.h"
#include "data_structs/queue.h"
#include "lib/errors.h"

void stack_tests(void) {
    printf("\nStack tests:\n");
    struct stack *s = Stack.new();

    Stack.push(s, 69);
    Stack.push(s, 420);
    Stack.push(s, 42069);

    int out0 = Stack.pop(s);
    int out1 = Stack.pop(s);
    printf("out: %d %d\n", out0, out1);

    Stack.free(&s);
    printf("s: %p\n", s);

    printf("\nEnd of stack tests\n");

    return;
}


void vector_tests(void) {
    printf("\nVector tests:\n");
    struct vector *vec = Vector.new();

    Vector.push_back(vec, 1);
    Vector.push_back(vec, 2);
    Vector.push_back(vec, 3);
    Vector.push_back(vec, 4);
    Vector.push_back(vec, 5);
    Vector.push_back(vec, 6);
    Vector.push_front(vec, 7);
    Vector.push_front(vec, 8);
    Vector.push_front(vec, 9);

    Vector.print(vec);

    Vector.pop_back(vec);
    Vector.pop_back(vec);
    Vector.pop_front(vec);
    Vector.pop_front(vec);
    Vector.pop_front(vec);

    Vector.print(vec);

    printf("\nEnd of vector tests\n");

    return;
}

void queue_tests(void) {
    printf("\nQueue tests:\n");
    struct queue *q = Queue.new();

    Queue.push(q, 1);
    Queue.push(q, 2);
    Queue.push(q, 3);
    Queue.push(q, 4);
    Queue.push(q, 5);

    printf("Front of Queue = %d\n", Queue.front(q));

    Queue.pop(q);
    Queue.pop(q);
    Queue.pop(q);
    
    printf("Front of Queue = %d\n", Queue.front(q));

    printf("Is Queue Empty : %d\n", Queue.is_empty(q));

    Queue.pop(q);
    Queue.pop(q);

    printf("Is Queue Empty : %d\n", Queue.is_empty(q));

    printf("\nEnd of queue tests\n");

    Queue.free(&q);

    return;
}

void cmd_line_handler(int argc, char **argv) {
    enum flags { STACK, VECTOR, QUEUE, FLAG_COUNT };
    int flag_arr[FLAG_COUNT] = {0};

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-Tall")) {
            for (int j = 0; j < FLAG_COUNT; j++)
                flag_arr[j] = 1;
        } else if (!strcmp(argv[i], "-Tstack"))
            flag_arr[STACK] = 1;
        else if (!strcmp(argv[i], "-Tvec"))
            flag_arr[VECTOR] = 1;
        else if (!strcmp(argv[i], "-Tqueue"))
            flag_arr[QUEUE] = 1;
        else
            warn("Unrecognized argument '%s'\n", argv[i]);
    }

    void (*fun_arr[])() = { stack_tests, vector_tests, queue_tests };

    for (int i = 0; i < FLAG_COUNT; i++)
        if (flag_arr[i])
            (*fun_arr[i])();
}

int main(int argc, char **argv) {
    cmd_line_handler(argc, argv);

    return 0;
}

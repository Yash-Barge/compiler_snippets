#include <stdio.h>

#include "data_structs/stack.h"
#include "data_structs/vector.h"
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


int main(void) {
    stack_tests();
    error("Hello! %d\n", 69);
    vector_tests();


    return 0;
}

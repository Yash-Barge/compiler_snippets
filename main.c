#import <stdio.h>
#import "data_structs/stack.h"

int main(void) {
    struct stack *s = Stack.new();

    Stack.push(s, 69);
    Stack.push(s, 420);
    Stack.push(s, 42069);

    int out0 = Stack.pop(s);
    int out1 = Stack.pop(s);
    printf("out: %d %d\n", out0, out1);

    Stack.free(&s);
    printf("s: %p\n", s);

    return 0;
}

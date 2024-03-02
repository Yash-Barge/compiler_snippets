#include <stdio.h>
#include <string.h>

// #include "data_structs/stack.h"
#include "data_structs/vector.h"
#include "data_structs/queue.h"
#include "data_structs/hashmap.h"
#include "data_structs/symbol_table.h"

// void stack_tests(void) {
//     printf("\nStack tests:\n");
//     struct stack *s = Stack.new();

//     Stack.push(s, 69);
//     Stack.push(s, 420);
//     Stack.push(s, 42069);

//     int out0 = Stack.pop(s);
//     int out1 = Stack.pop(s);
//     printf("out: %d %d\n", out0, out1);

//     Stack.free(&s);
//     printf("s: %p\n", s);

//     printf("\nEnd of stack tests\n");

//     return;
// }


// void vector_tests(void) {
//     printf("\nVector tests:\n");
//     struct vector *vec = Vector.new();

//     Vector.push_back(vec, 1);
//     Vector.push_back(vec, 2);
//     Vector.push_back(vec, 3);
//     Vector.push_back(vec, 4);
//     Vector.push_back(vec, 5);
//     Vector.push_back(vec, 6);
//     Vector.push_front(vec, 7);
//     Vector.push_front(vec, 8);
//     Vector.push_front(vec, 9);

//     Vector.sort(vec, 0);

//     Vector.print(vec);

//     Vector.pop_back(vec);
//     Vector.pop_back(vec);
//     Vector.pop_front(vec);
//     Vector.pop_front(vec);
//     Vector.pop_front(vec);

//     Vector.print(vec);

//     printf("\nEnd of vector tests\n");

//     return;
// }

void queue_tests(void) {
//     printf("\nQueue tests:\n");
//     struct queue *q = Queue.new();

//     Queue.push(q, 1);
//     Queue.push(q, 2);
//     Queue.push(q, 3);
//     Queue.push(q, 4);
//     Queue.push(q, 5);

//     printf("Front of Queue = %d\n", Queue.front(q));

//     Queue.pop(q);
//     Queue.pop(q);
//     Queue.pop(q);
    
//     printf("Front of Queue = %d\n", Queue.front(q));

//     printf("Is Queue Empty : %d\n", Queue.is_empty(q));

//     Queue.pop(q);
//     Queue.pop(q);

//     printf("Is Queue Empty : %d\n", Queue.is_empty(q));

//     printf("\nEnd of queue tests\n");

//     Queue.free(&q);

//     return;
}

void hashmap_tests(void){
//     printf("\nHashmap tests:\n");
//     struct hashmap *hm = Hashmap.new(11);

//     Hashmap.insert(&hm, "uwu");
//     Hashmap.insert(&hm, "owo");
//     Hashmap.insert(&hm, "ara-ara");
//     Hashmap.insert(&hm, "YashBargeGhot");
//     Hashmap.insert(&hm, "CoCo");
//     Hashmap.insert(&hm, "CoNe");
//     Hashmap.insert(&hm, "def");
//     Hashmap.insert(&hm, "abc");
//     Hashmap.insert(&hm, "abc");
//     Hashmap.insert(&hm, "43");

//     printf("Search for Value Yash = %d\n", Hashmap.search(hm, "Yash"));
//     printf("Search for Value YashBargeGhot = %d\n", Hashmap.search(hm, "YashBargeGhot"));
//     printf("Search for Value Shubham = %d\n", Hashmap.search(hm, "Shubham"));
//     printf("Search for Value CoNe = %d\n", Hashmap.search(hm, "CoNe"));

//     Hashmap.erase(hm, "jsdijdijdsijdsijs");
//     Hashmap.erase(hm, "YashBargeGhot");
//     Hashmap.erase(hm, "CoNe");

//     printf("\nErasing...\n");

//     printf("Search for Value Yash = %d\n", Hashmap.search(hm, "Yash"));
//     printf("Search for Value YashBargeGhot = %d\n", Hashmap.search(hm, "YashBargeGhot"));
//     printf("Search for Value Shubham = %d\n", Hashmap.search(hm, "Shubham"));
//     printf("Search for Value CoNe = %d\n", Hashmap.search(hm, "CoNe"));

//     printf("\nEnd of Hashmap tests\n");

//     Hashmap.free(&hm);

//     return;
    
}

void symbol_table_tests(void){
    printf("\nSymbol Table Tests:\n\n");
    struct symbol_table* sym_table = SymbolTable.new(50);
    // SymbolTable.insert(&sym_table, "b");
    printf("BEFORE INSERTION\n");
    SymbolTable.insert(&sym_table, "57", TK_NUM);
    SymbolTable.insert(&sym_table, "57.00", TK_RNUM);
    printf("INSERTION 0\n");
    SymbolTable.insert(&sym_table, "b2b6", TK_ID);
    printf("INSERTION 1\n");
    SymbolTable.insert(&sym_table, "b2b5", TK_ID);
    printf("INSERTION 2\n");
    SymbolTable.insert(&sym_table, "b2b4", TK_ID);
    printf("INSERTION 3\n");
    SymbolTable.insert(&sym_table, "b2b3", TK_ID);
    printf("INSERTION 4\n");
    SymbolTable.insert(&sym_table, "b2b2", TK_ID);
    printf("INSERTION 5\n");
    SymbolTable.insert(&sym_table, "b2c7", TK_ID);
    printf("INSERTION 6\n");
    SymbolTable.insert(&sym_table, "b2c6", TK_ID);
    printf("INSERTION 7\n");
    SymbolTable.insert(&sym_table, "while", TK_WHILE);
    printf("INSERTION 8\n");
    SymbolTable.insert(&sym_table, "union", TK_UNION);
    printf("INSERTION 9\n");
    SymbolTable.insert(&sym_table, "endunion", TK_ENDUNION);

    printf("Search for Value b2b7 = %d\n", SymbolTable.search(sym_table, "b2b7", TK_ID));
    printf("Search for Value b2b6 = %d\n", SymbolTable.search(sym_table, "b2b6", TK_ID));
    printf("Search for Value Shubham = %d\n", SymbolTable.search(sym_table, "Shubham", TK_FIELDID));
    printf("Search for Value while = %d\n", SymbolTable.search(sym_table, "while", TK_WHILE));


    SymbolTable.free(&sym_table);

    struct symbol_table* init_symbol_table = SymbolTable.init();

    printf("\n\nSearch for Value ~ = %d\n", SymbolTable.search(init_symbol_table, "~", TK_NOT));
    printf("Search for Value write = %d\n", SymbolTable.search(init_symbol_table, "write", TK_WRITE));
    printf("Search for Value else = %d\n", SymbolTable.search(init_symbol_table, "else", TK_ELSE));
    printf("Search for Value while = %d\n", SymbolTable.search(init_symbol_table, "while", TK_WHILE));
    printf("Search for Value <--- = %d\n", SymbolTable.search(init_symbol_table, "<---", TK_ASSIGNOP));
    printf("Search for Value != = %d\n", SymbolTable.search(init_symbol_table, "!=", TK_NE));
    printf("Search for Value - = %d\n", SymbolTable.search(init_symbol_table, "-", TK_MINUS));


    SymbolTable.free(&init_symbol_table);
    printf("\nEnd of Symbol Table\n");
    return;
}

void cmd_line_handler(int argc, char **argv) {
    enum flags { QUEUE, HASHMAP, SYMBOL_TABLE, FLAG_COUNT };
    int flag_arr[FLAG_COUNT] = {0};

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-Tall")) {
            for (int j = 0; j < FLAG_COUNT; j++)
                flag_arr[j] = 1;
        }
        // else if (!strcmp(argv[i], "-Tvec"))
        //     flag_arr[VECTOR] = 1;
        else if (!strcmp(argv[i], "-Tqueue"))
            flag_arr[QUEUE] = 1;
        else if (!strcmp(argv[i], "-Thashmap"))
            flag_arr[HASHMAP] = 1;
        else if (!strcmp(argv[i], "-Tsymboltable"))
            flag_arr[SYMBOL_TABLE] = 1;
        // else
        //     warn("Unrecognized argument '%s'\n", argv[i]);
    }

    void (*fun_arr[])() = {queue_tests, hashmap_tests, symbol_table_tests }; // stack_tests, vector_tests, 

    for (int i = 0; i < FLAG_COUNT; i++)
        if (flag_arr[i])
            (*fun_arr[i])();
}

int main(int argc, char **argv) {
    cmd_line_handler(argc, argv);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "first_follow_sets.h"
#include "set.h"

// void parse_grammar_file(FILE* fp){
//     if(getchar(fp) == '<'){

//     }
    
// }


int main(void){
    printf("\n\nSET TESTS\n\n");

    struct set* st = Set.new();

    printf("\n\n SET CREATED SUCCESSFULLY\n\n");
    Set.insert(st, TK_OP);
    Set.insert(st, TK_CL);
    Set.insert(st, TK_EPSILON);
    Set.insert(st, TK_ASSIGNOP);
    Set.insert(st, TK_MUL);
    Set.insert(st, TK_DIV);

    printf("\n\nINSERTIONS DONE\n\n");

    Set.print(st);

    printf("\n\nPRINTING DONE\n\n");

    Set.free(&st);
    
    return 0;
}
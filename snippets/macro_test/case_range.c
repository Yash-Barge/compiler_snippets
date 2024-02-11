#include <stdio.h>
#include <stdlib.h>

#define CASE_RANGE(X, Y) case X ... Y



int main(void){
    for(int i=0; i<20; i++){
        switch (i) {
            CASE_RANGE(1, 5):
                printf("%d is between 1 and 5\n", i);
                break;
            CASE_RANGE(6, 10):
                printf("%d is between 6 and 10\n", i);
                break;
            default:
                printf("Not in range\n");
                break;
        }
    }
}
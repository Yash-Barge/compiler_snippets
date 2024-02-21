#include <stdio.h>
#include <stdlib.h>
#include "first_follow_sets.h"
#include "set.h"

#define MAXSIZE 2048

int get_non_terminals(FILE* fp){
    char buffer[MAXSIZE];
    FILE* fp2 = fopen("NT.txt", "w+");
    int line_count = 0;
    while(fgets(buffer, MAXSIZE, fp)){
        for(int i=0; i<strlen(buffer); ){
            if(buffer[i++] == '<'){
                char temp[MAXSIZE];
                int j=0;
                line_count++;
                while(buffer[i] != '>'){
                    temp[j++] = buffer[i++];
                }
                temp[j++] = '\n';
                temp[j] = '\0';
                fprintf(fp2, "%s",temp);
                memset(temp, '\0', MAXSIZE);
                break;
            }
        }
        memset(buffer, '\0', MAXSIZE);
    }
    fclose(fp2);
    return line_count;
}

void get_rhs_of_rules(FILE* fp){
    char buffer[MAXSIZE];
    FILE* fp2 = fopen("Rules.txt", "w+");
    memset(buffer, '\0', MAXSIZE);
    while(fgets(buffer, MAXSIZE, fp)){
        for(int i=0; i<strlen(buffer); ){
            if(buffer[i++] == '='){
                i+=3;
                char temp[MAXSIZE];
                memset(temp, '\0', MAXSIZE);
                int k = i;
                for(int j = i; j<strlen(buffer); j++){
                    temp[j-i] = buffer[k++];
                }
                fprintf(fp2, "%s", temp);
                printf("%s\n", temp);
            }
        }
        memset(buffer, '\0', MAXSIZE);
    }
    fclose(fp2);
    return;
}

void find_first(struct set **st, FILE* nt_ptr, FILE* rules_ptr, int num_nt){

}

// ~

int main(void){
    FILE* fp = fopen("grammar.txt", "r");
    FILE* fp2 = fopen("grammar.txt", "r");
    int num_of_non_terminals = 0;
    num_of_non_terminals = get_non_terminals(fp);
    get_rhs_of_rules(fp2);
    fclose(fp);
    fclose(fp2);
    

    struct set* st[num_of_non_terminals];
    return 0;
}
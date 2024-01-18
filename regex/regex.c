#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



#include "regex.h"

char* make_range(char* reg){ 
    // reg of form [x-y]* or [x-y]+ or [x-y]
    int start = reg[1];
    int end = reg[3];
    char* range = malloc((end - start + 2) * sizeof(char)); // todo - check for null character
    
    if(start > end) {
        printf("Start of range can't be more than end");
        return "-1";
    }
    
    for(int i=start; i<=end; i++){
        range[i-start] = (char)(reg[1] + i - start);
    }

    range[end - start + 1] = '\0';
    return range;
}

long long find_freq(char* regex) {
    long long freq = 0;
    for(int i=0; i<strlen(regex); i++){
        if(regex[i] == ']' && regex[i+1] == '[') freq = freq*10 + 1; // [x-y]
        else if(regex[i] == ']' && regex[i+1] == '*') freq = freq*10 + 2; // [x-y]*
        // for [x-y]+ use [x-y][x-y]*
    }
    long long rev_freq = 0;
    while(freq != 0){
        int rem = freq % 10;
        rev_freq = rev_freq * 10 + rem;
        freq = freq / 10;
    }
    return rev_freq;
}

char* substr(char* regex, int start) {
    char* sub = malloc(6 * sizeof(char));
    for(int i=0; i<5; i++){
        sub[i] = regex[start + i];
    }
    return sub;
}

char** divide_regex(char* regex){
    int num_of_parts = 0;
    for(int i=0; i<strlen(regex); i++){
        if(regex[i] == ']') num_of_parts++;
    }
    char** parts = calloc(num_of_parts, sizeof(char*));
    int ptr = 0;    
    for(int i=0; i<num_of_parts;){
        if(regex[ptr] == '['){
            char* subs = substr(regex, ptr);
            char* ran = make_range(subs);
            parts[i] = ran;
            i++;
        }
        ptr++;
    }
    return parts;
}
 
bool search(char* range, char ele){
    return ((ele <= range[strlen(range) - 1]) && (ele >= range[0]));
}

bool end_of_regex(long long freq){
    while (freq){
        if(freq % 10 == 1) return false;
        freq /= 10;
    }
    return true;
} 

bool check(char* regex, char* expr){
    char** parts = divide_regex(regex);
    int expr_ptr = 0;
    int id = 0;

    long long freq = find_freq(regex);

    while(true){
        printf("%c -> ", expr[expr_ptr]);
        
        if(id == sizeof(parts)) break;
        else if(freq == 0 && expr_ptr != strlen(expr) - 1) {
            printf("in cond 1\n");
            printf("At end of regex, but expression is still left.\n");
            printf("Invalid.\n");
            return 0;
        }
        else if(expr_ptr == strlen(expr) - 1 && !end_of_regex(freq/10)) {
            printf("in cond 2\n");
            printf("At end of expression, but regex has more terms.\n");
            printf("Invalid.\n");
            return 0;
        }
        else if(expr_ptr == strlen(expr) - 1 && end_of_regex(freq/10)) {
            break;
        }
        else if(freq%10 == 1 && search(parts[id], expr[expr_ptr])){
            printf("in cond 3\n");
            id++;
            expr_ptr++;
            freq/=10;
        }
        else if(freq%10 == 1 && !search(parts[id], expr[expr_ptr])){
            printf("in cond 4\n");
            printf("Failed because of absence of necessary part of regex.\n");
            return 0;
        }
        else if(freq%10 == 2 && search(parts[id], expr[expr_ptr])){
            printf("in cond 5\n");
            expr_ptr++;
        }
        else if(freq%10 == 2 && !search(parts[id], expr[expr_ptr])){
            printf("in cond 6\n");
            id++;
            freq/=10;
        }
    }

    if(end_of_regex(freq)) {
        printf("Valid\n");
        return 1;
    }
    printf("Invalid\n");
    return 0;

}

int main(void) {

    char* regex = "[2-7][2-7][2-7][2-7]";
    check(regex, "222");


    // char* regex = "[b-d][2-7][b-d]*[2-7]*";
    // bool check1 = check(regex, "b2565");
    // printf("\n\n\n");
    // bool check2 = check(regex, "b2bccd");
    // printf("\n\n\n");
    // bool check3 = check(regex, "b2bccddc7667");
    // printf("\n\n\n");
    // printf("\n\n\n");
    // bool check5 = check(regex, "2");
    // printf("\n\n\n");
    // bool check6 = check(regex, "b");
}


// void printVal(int x){
//     if(x == 0) printf("Match\n");
//     else if(x == REG_NOMATCH) printf("Not match\n");
//     else printf("fuck off\n");
// }

// int main(){
//     regex_t variableId;
//     const char* id = "[:number:]";
//     int compilationStatus = regcomp(&variableId, id, 0);
//     if(compilationStatus == 0) {
//         printf("Compilation Failed\n");
//         return -1;
//     }

//     printVal(regexec(&variableId, "b2", 0, NULL, 0));
//     printVal(regexec(&variableId, "b2b", 0, NULL, 0));
//     printVal(regexec(&variableId, "b2d", 0, NULL, 0));
//     printVal(regexec(&variableId, "b2c", 0, NULL, 0));
//     printVal(regexec(&variableId, "b2bcd", 0, NULL, 0));
//     printVal(regexec(&variableId, "b2bbbbb", 0, NULL, 0));
// }
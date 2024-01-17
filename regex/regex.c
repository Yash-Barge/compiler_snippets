#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
        else if(regex[i] == ']' && regex[i+1] == '+') freq = freq*10 + 3; // [x-y]+
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
 
bool check(char* regex, char* expr){
     
}

int main(void) {
    char** range = divide_regex("[b-f][3-6]*[1-9]+");
    for(int i=0; i<(sizeof(range)); i++){
        printf("%d\t%s\n", i, range[i]);
    }
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
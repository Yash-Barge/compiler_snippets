#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "regex.h"



/**
 * @param reg One singular regular expression 
 * @return char* A character array of size 3, 0th element being the start, 1st element being the end and 3rd character being '\0'
 */
char* make_range(char* reg){ 
    // reg of form [x-y]* or [x-y] or [alp]
    if(reg[2] != 'l' && reg[2] != '-') {
        printf("INVALID REGEX.\n");
        return NULL;
    }
    if(reg[2] == 'l'){
        // [alp] condition
        char* range = malloc(3 * sizeof(char));
        range[0] = 'a';
        range[1] = 'Z';
        range[2] = '\0';
        return range;
    }
    
    
    if(reg[1] > reg[3]) {
        printf("Start of range can't be more than end.\n");
        return NULL;
    }

    char* range = malloc(3 * sizeof(char));

    range[0] = reg[1];
    range[1] = reg[3];
    range[2] = '\0';
    return range;
}



/**
 * @param regex the regular expression 
 * @return long long the frequency of parts of the regex, in reverse order (leftmost part of regex = LSB of freq)
 */
long long find_freq(char* regex) {

    // if regex = [a-z][a-z][1-5]*[a-z], freq = 1211 (1 for parts without *, 2 for parts with *) 
    
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

/**
 * @param regex The regular expression
 * @param start The starting index
 * @param output Pass in char array of at least buffer size 6. The substring of the regex, starting from index start, and taking 5 characters.
*/
void substr(char *regex, int start, char *output) {
    for(int i=0; i<5; i++){
        output[i] = regex[start + i];
    }
    output[5] = '\0';
    return;
}



/**
 * @param regex The regular expression, that has to be divided into it's components
 * @param size the number of components
 * @return char** An array of character pointers of the individual components of the regex
 */
char** divide_regex(char* regex, int *size){

    //if regex = [a-z][0-9]*[p-q]*, parts[0] = [a-z], parts[1] = [0-9], parts[2] = [p-q]

    int num_of_parts = 0;
    for(int i=0; i<strlen(regex); i++){
        if(regex[i] == ']') num_of_parts++;
    }
    char** parts = calloc(num_of_parts, sizeof(char*));
    int ptr = 0;    
    for(int i=0; i<num_of_parts;){
        if(regex[ptr] == '['){
            char substr_out[6];
            substr(regex, ptr, substr_out);
            char* ran = make_range(substr_out);
            parts[i] = ran;
            i++;
        }
        ptr++;
    }
    *size = num_of_parts;
    return parts;
}



/**
 * @param range the range of characters as generated from make_range() 
 * @param ele character that has to be checked, whether it belongs in that range or not
 * @return true if the character is present in that range
 * @return false if the character is not present in that range
 */
bool search(char* range, char ele){
    if(range[0] == 'a' && range[1] == 'Z'){
        return ((ele <= 'Z' && ele >= 'A') || (ele <= 'z' && ele >= 'a'));
    }
    return ((ele <= range[1]) && (ele >= range[0]));
}



/** 
 * @param parts Divided regex that needs to be freed
 * @param num_of_parts Number of elements in the divided regex 
 */
void freeParts(char** parts, int num_of_parts){
    for(int i=0; i<num_of_parts; i++){
        free(parts[i]);
    }
    free(parts);
}



/**
 * @param freq The frequency of elements, as generated from find_freq()
 * @return true if the freq number can denote a theoretical end of the regex
 * @return false if the freq number can not be the theoretical end of the regex
 */
bool end_of_regex(long long freq){
    /*
        if regex = [a-z], and we have read one char, we are at the end
        if regex = [0-9]*, and we have read no chars, we can be at the end
    */
    while (freq){
        if(freq % 10 == 1) return false;
        freq /= 10;
    }
    return true;
} 



/**
 * @param regex The input regex, against which expr is to be checked 
 * @param expr The expression, that has to be checked if it can be made from the regex
 * @return 0 if expression can't be generated from the regex, else returns 1
 */
int check(char* regex, char* expr){
    int parts_length;
    char** parts = divide_regex(regex, &parts_length);
    int expr_ptr = 0;
    int id = 0;

    long long freq = find_freq(regex);

    while(true){
        // printf("%d %d\n", id, expr_ptr);
        
        if(end_of_regex(freq) && (strcmp(expr, "") == 0)){
            // Empty string part of language.
            // printf("PASS1\n");
            freeParts(parts, parts_length);
            return 1;
        }
        if(freq == 0 && expr_ptr != strlen(expr) - 1) {
            // At end of regex, but expression is still left.
            // printf("FAIL1\n");
            freeParts(parts, parts_length);
            return 0;
        }
        else if(expr_ptr == strlen(expr) - 1 && !end_of_regex(freq/10)) {
            // At end of expression, but regex has more terms.
            // printf("FAIL2\n");
            freeParts(parts, parts_length);
            return 0;
        }
        else if(expr_ptr == strlen(expr) - 1 && end_of_regex(freq/10)) {
            int flag = 0;
            while(id < parts_length){
                if(search(parts[id], expr[expr_ptr])) {
                    flag = 1;
                    break;
                }
                id++;
                freq /= 10;
                // printf("%d %d\n", id, expr_ptr);

            }
            if(flag) break;
            // printf("Invalid\n");
            // printf("FAIL3\n");
            freeParts(parts, parts_length);
            return 0;
            
        }
        else if(freq%10 == 1 && search(parts[id], expr[expr_ptr])){
            // printf("OP1\n");
            id++;
            expr_ptr++;
            freq/=10;
        }
        else if(freq%10 == 1 && !search(parts[id], expr[expr_ptr])){
            // Fail because of absence of necessary part of regex. ([0-9][alp]* with input abc, then [0-9] is necessary part of regex)
            // printf("FAIL4\n");
            freeParts(parts, parts_length);
            return 0;
        }
        else if(freq%10 == 2 && search(parts[id], expr[expr_ptr])){
            // printf("OP2\n");
            expr_ptr++;
        }
        else if(freq%10 == 2 && !search(parts[id], expr[expr_ptr])){
            // printf("OP3\n");
            id++;
            freq/=10;
        }
    }
    freeParts(parts, parts_length);
    // printf("AT END\n");
    return end_of_regex(freq);
}

// int main(void) {

//     // char *regex = "[alp]*[1-9]*";

//     // check(regex, "testing123");
//     // check(regex, "yo");
//     // check(regex, "");
//     // check(regex, "12345");
//     // check(regex, "12345abcd");
//     // check(regex, "12345a");

//     char *regex = "[b-d][2-7][b-d]*[2-7]";

//     // printf("%s\n", check(regex, "a93") ? "Valid" : "Invalid");
//     // printf("%s\n", check(regex, "zzz") ? "Valid" : "Invalid");
//     // printf("%s\n", check(regex, "zzz325") ? "Valid" : "Invalid");
//     printf("%s\n", check(regex, "b2b7") ? "Valid" : "Invalid");
    
//     // char* regex = "[2-7][2-7][2-7][2-7]";
//     // check(regex, "222");


//     // char* regex = "[b-d][2-7][b-d]*[2-7]*";
//     // bool check1 = check(regex, "b2565");
//     // printf("\n\n\n");
//     // bool check2 = check(regex, "b2bccd");
//     // printf("\n\n\n");
//     // bool check3 = check(regex, "b2bccddc7667");
//     // printf("\n\n\n");
//     // printf("\n\n\n");
//     // bool check5 = check(regex, "2");
//     // printf("\n\n\n");
//     // bool check6 = check(regex, "b");
// }

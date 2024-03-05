/*
    GROUP NUMBER - 27
    Shubham Gupta - 2021A7PS0468P
    Yash Sangram Barge - 2021A7PS0006P
    Venkatavihan Devaki - 2021A7PS0429P
    Priyank Shethia - 2021A7PS0657P
    Aditya Khandelwal - 2021A7PS2422P
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "lexerDef.h"
#include "parserDef.h"
#include "parser.h"
#include "others.h"

#define IS_DELIM(tok_type) (tok_type == TK_SEM || tok_type == TK_ENDUNION || tok_type == TK_ENDWHILE || tok_type == TK_ENDIF || tok_type == TK_END || tok_type == TK_ENDRECORD)
#define NO_ERRORS (!get_lexer_error_count() && !get_parser_error_count())

#define MAXSIZE 512

// TODO: Add error handling to all of this

/**
 * @brief Checks if `substr` is a substring within `str`
 * @return Index of first occurence of substr in str, -1 if not found
 */
int substring(const char *restrict const str, const char *restrict const substr) {
    int str_len = strlen(str);
    int substr_len = strlen(substr);

    int i;
    for (i = 0; i < str_len - substr_len + 1; i++)
        if (!strncmp(str + i, substr, substr_len))
            break;

    return i == str_len - substr_len + 1 ? -1 : i;
}

/**
 * @return Returns number of occurences of `substr` within `str`
 */
int substring_count(const char *restrict const str, const char *restrict const substr) {
    int ret = 0;
    unsigned int offset = 0;

    while (offset < strlen(str)) {
        int temp = substring(str + offset, substr);
        if (temp == -1) {
            break;
        }
        offset += temp + 1;
        ret++;
    }

    return ret;
}

/**
 * @brief Helper function for `make_grammar()`. Should not be used outside of it.
 * @param raw_prod_rule String production rule in the form of `<A> ===> <B> <C> TK_EXAMPLE ...`
 * @param index Pointer to index, starts locating next terminal from that index
 * @return 
 */
char *get_next_t_or_nt(const char *restrict const raw_prod_rule, unsigned int *index) {
    while (*index < strlen(raw_prod_rule) && raw_prod_rule[*index] == ' ') {
        if (raw_prod_rule[*index] == ' ')
            (*index)++;
        else
            *index += 4;
    }

    if (*index == strlen(raw_prod_rule))
        return NULL;
    
    if (!substring(raw_prod_rule + *index, "===>")) {
        (*index) += 4;
        return "===>";
    }

    if (raw_prod_rule[*index] == '|') {
        (*index)++;
        return "|";
    }

    int start_index = *index, nt = 0;

    if (raw_prod_rule[start_index] == '<') {
        nt = 1;
        start_index = ++(*index);

        while (*index < strlen(raw_prod_rule) && raw_prod_rule[*index] != '>')
            (*index)++;
    } else if (!strncmp(raw_prod_rule + start_index, "EPS", 3)) {
        *index += 3;
        // all this is done to avoid segfaults when free-ing stringified tokens
        char temp[] = "TK_EPSILON";
        char *temp2 = malloc(sizeof(*temp2) * (strlen(temp) + 1));
        strcpy(temp2, temp);
        return temp2;
    } else {
        while (*index < strlen(raw_prod_rule) && (raw_prod_rule[*index] == '_' || (raw_prod_rule[*index] >= 'A' && raw_prod_rule[*index] <= 'Z')))
            (*index)++;
    }

    char *ret = malloc(sizeof(*ret) * (*index - start_index + 1));
    strncpy(ret, raw_prod_rule + start_index, *index - start_index);
    ret[*index - start_index] = '\0';

    // to go past the last '>' character
    if (nt)
        (*index)++;
    
    return ret;
}

/**
 * @brief Helper function for `make_grammar()`. Should not be used outside of it. Turns terminals into the enum from `token_names`, turns non-terminals
 *        into integers starting from `TK_COUNT`.
 * @param g (Incomplete) grammar
 * @param t_or_nt The (non-)terminal to enumify
 * @return An integer, that's < `TK_COUNT` if it is a terminal, or >= `TK_COUNT` if it's not. Corresponds to index in `g->rules[ret_val - TK_COUNT]`.
 */
int enumify_t_or_nt(struct grammar *g, const char *restrict const t_or_nt) {
    if (t_or_nt[0] == 'T') {
        int enum_val = tok_enumify(t_or_nt);

        if (enum_val == -1)
            exit(1); // ERROR
        
        return enum_val;
    }

    for (int i = 0; i < g->rule_count; i++)
        if (!strcmp(t_or_nt, g->rules[i].lhs))
            return i + TK_COUNT;
        
    // ERROR
    fprintf(stderr, "\033[31mError parsing grammar:\033[0m %s\nThis non-terminal does not exist on LHS of any grammar production rule\n", t_or_nt);

    exit(1);
}

/**
 * @brief Generate a grammar array form a file
 * 
 * @param grammar_file_path The path to the file that has the grammar rules
 * @return struct grammar* A pointer to the memory allocated to the grammar struct
 */
struct grammar *make_grammar(const char *restrict const grammar_file_path) {
    FILE *grammar_file = fopen(grammar_file_path, "r");

    struct vector_string *prod_rules_raw = VectorString.new();

    // TODO: Redo this properly
    char buffer[MAXSIZE];
    while (fgets(buffer, sizeof(buffer), grammar_file)) {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        if (buffer[strlen(buffer) - 1] == '\r')
            buffer[strlen(buffer) - 1] = '\0';

        if (!strlen(buffer))
            continue;
        else if (substring_count(buffer, "===>")) // not empty and grammar rule
            VectorString.push_back(prod_rules_raw, buffer);
        else
            fprintf(stderr, "\033[33mWARNING:\033[0m Line %s is not counted as `===>` not found in line\n", buffer);
    }

    struct grammar *ret = malloc(sizeof(*ret) + sizeof(struct grammar_rule) * VectorString.size(prod_rules_raw));
    ret->rule_count = VectorString.size(prod_rules_raw);

    // Initializes only lhs and rhs_count
    for (int i = 0; i < ret->rule_count; i++) {
        const char *const temp = VectorString.at(prod_rules_raw, i);

        unsigned int end_of_lhs = 0;
        ret->rules[i].lhs = get_next_t_or_nt(temp, &end_of_lhs);
        ret->rules[i].rhs_count = 1 + substring_count(temp, "|");
    }

    // Computes rhs for each grammar rule
    for (int i = 0; i < ret->rule_count; i++) {
        const char *const temp = VectorString.at(prod_rules_raw, i);
        int index = substring(temp, "===>") + 4;

        ret->rules[i].rhs = malloc(sizeof(struct vector_int *) * ret->rules[i].rhs_count);
        for (int j = 0; j < ret->rules[i].rhs_count; j++)
            ret->rules[i].rhs[j] = VectorInt.new();
        
        int j = 0;
        while (index < (int) strlen(temp)) {
            char *t_or_nt = get_next_t_or_nt(temp, (unsigned int *) &index);

            if (t_or_nt == NULL)
                continue;

            if (!strcmp(t_or_nt, "|")) {
                j++;
                continue;
            }

            VectorInt.push_back(ret->rules[i].rhs[j], enumify_t_or_nt(ret, t_or_nt));
            free(t_or_nt);
        }
    }

    VectorString.free(&prod_rules_raw);

    fclose(grammar_file);

    return ret;
}

/**
 * @brief Free the memory allocated to the grammar struct
 * 
 * @param p_g Pointer to the pointer to the Grammar Struct
 */
void free_grammar(struct grammar **p_g) {
    assert(p_g != NULL);
    assert(*p_g != NULL);

    struct grammar *g = *p_g;

    for (int i = 0; i < g->rule_count; i++) {
        free(g->rules[i].lhs);

        for (int j = 0; j < g->rules[i].rhs_count; j++)
            VectorInt.free(&g->rules[i].rhs[j]);
            
        free(g->rules[i].rhs);
    }

    free(g);
    *p_g = NULL;

    return;
}

const char *t_or_nt_string(struct grammar *g, int enumified_t_or_nt) {
    if (enumified_t_or_nt == -1)
        return "$";
    return enumified_t_or_nt < TK_COUNT ? TOK_STRING[enumified_t_or_nt] : g->rules[enumified_t_or_nt - TK_COUNT].lhs;
}

/**
 * @brief Helper Function for generation of First Sets
 * 
 * @param g The grammar 
 * @param st A pointer to the array of sets, used for recursion
 * @param index The of the grammar rule (Or non terminal)
 * @return struct set* The return value of the set at index
 */
struct set *first_helper(struct grammar* g, struct set** st, int index){
    if (st[index] == NULL) {
        st[index] = Set.new(); // Initialise the Set

        for (int i=0; i<g->rules[index].rhs_count; i++) {
            if (VectorInt.at(g->rules[index].rhs[i], 0) < TK_COUNT) {
                Set.insert(st[index], VectorInt.at(g->rules[index].rhs[i], 0)); // If the first token in the RHS of a production rule is a terminal, add it to the first set
            } else {
                int current_index = 0; 

                /* 
                    Current index of the non terminal we are finding the first of
                    if <A> ===> <B><C><D><E>
                    To find first(A), we will need the first(B), and if that has an epsilon, we will move onto finding the first(C)
                    if we are finding first(C), current_index = 1
                */

                while (current_index < VectorInt.size(g->rules[index].rhs[i])) { // Iterate over the Right hand side of the rule for all Non terminals
                    if (VectorInt.at(g->rules[index].rhs[i], current_index) < TK_COUNT) { // If it is a terminal, add it to the set, and break out of the while loop
                        Set.insert(st[index], VectorInt.at(g->rules[index].rhs[i], current_index));
                        break;
                    }

                    // Now we have established that the token at index value = current_index is a non terminal

                    struct set *temp = first_helper(g, st, VectorInt.at(g->rules[index].rhs[i], current_index) - TK_COUNT); // Recursive call on the non terminals
                    int contains_epsilon = 0; // Flag that indicates if the non terminal has epsilon or not

                    for (int j = 0; j < Set.size(temp); j++) { // Iterate over the elements of the first(Non_Terminal)
                        if (current_index == VectorInt.size(g->rules[index].rhs[i]) - 1 || Set.at(temp, j) != TK_EPSILON) // If it is the last rule, blindly add everything in it's first set to the original first set 
                            Set.insert(st[index], Set.at(temp, j));
                        else
                            contains_epsilon = 1; // Indicates the non terminal has epsilon
                    }

                    if (contains_epsilon)
                        current_index++; // Move to the next index if it has epsilon, and continue checking
                    else
                        break;
                }
            }
        }
    }

    return st[index];
}

/**
 * @brief Used to generate the First Sets
 * 
 * @param g The grammar rules
 * @return struct set** An Array of Set ADT's, each corresponding to the first set of a certain non terminal
 */
struct set** generate_first(struct grammar* g) {
    struct set** first = malloc(sizeof(struct set*) * g->rule_count);
    for(int i=0; i<g->rule_count; i++){
        first[i] = NULL;
    }

     for(int i=0; i<g->rule_count; i++){
        if(!first[i]){
            first_helper(g, first, i);
        }
     }


    return first;
}

/**
 * @brief Used to preprocess data before passing it for Follow Set Generation
 * 
 * @param g The grammar 
 * @return struct set** An array of sets, where the i'th index of the array has a set of all grammar rules which have that non terminal in the RHS 
 */
struct set** follow_preprocess(struct grammar* g){
    struct set** processed_data = malloc(sizeof(struct set*) * g->rule_count);

    for(int i=0; i<g->rule_count; i++){
        processed_data[i] = Set.new();
    }

    for (int i = 0; i < g->rule_count; i++) {
        for (int j = 0; j < g->rules[i].rhs_count; j++) {
            for (int k = 0; k < VectorInt.size(g->rules[i].rhs[j]); k++) {
                if (VectorInt.at(g->rules[i].rhs[j], k) >= TK_COUNT) {
                    Set.insert(processed_data[VectorInt.at(g->rules[i].rhs[j], k) - TK_COUNT], i + TK_COUNT);
                }
            }
        }
    }


    return processed_data;
}

/**
 * @brief Helper funciton used for calculation of Follow Sets
 * 
 * @param g The grammar 
 * @param st A pointer to the array of sets, used for recursion
 * @param index The of the grammar rule (Or non terminal)
 * @param proc The processed Data
 * @param first The first sets
 * @return struct set* The return value of the set at index
 */
struct set *follow_helper(struct grammar* g, struct set** st, int index, struct set** proc, struct set** first){
    if (st[index] == NULL) {
        st[index] = Set.new(); // Initialise the Set
        for(int i=0; i<Set.size(proc[index]); i++){
            int NT = index + TK_COUNT; // The non terminal whose Follow we are calculating
            int grammarID = Set.at(proc[index], i) - TK_COUNT; // The grammar rule which we are currently referring to

            for (int a = 0; a < g->rules[grammarID].rhs_count; a++) {
                for(int j=0; j < VectorInt.size(g->rules[grammarID].rhs[a]); j++){
                    if (NT == VectorInt.at(g->rules[grammarID].rhs[a], j)) {
                        if (j == VectorInt.size(g->rules[grammarID].rhs[a]) - 1) { 

                            /*
                                If we are at the end of the production rule, say A ===> BCDE, and we want to find the 
                                follow of E, then all elements in Follow(A) belong to Follow(E)

                                However, if a production like A ===> BCA exists, we would be in a dilemma if we use the above
                                logic, and so don't follow it
                                
                            */
                            if(strcmp(g->rules[grammarID].lhs, t_or_nt_string(g, NT))){ 

                                /*
                                    Execute code in this if condition if the transition is not of the form A ===> BCA
                                */

                                struct set* temp = follow_helper(g, st, grammarID, proc, first);

                                for (int k = 0; k < Set.size(temp); k++)
                                    Set.insert(st[index], Set.at(temp, k));
                            }
                                
                        } else if (VectorInt.at(g->rules[grammarID].rhs[a], j+1) < TK_COUNT) {

                            /*
                                If a Non terminal is followed by a terminal, we can simply add those terminals to the follow
                                of the non terminal, and exit out of the loop
                            */

                            Set.insert(st[index], VectorInt.at(g->rules[grammarID].rhs[a], j + 1));
                            break;
                        } else {

                            /*
                                This is the case when we have a non terminal following a non terminal, and that can be generating epsilon
                            */

                            int currInd = j+1; // Our current index of the following non terminal. As the terminal whose follow we want is at index j, the followed non terminal is at index j+1
                            while (1) {
                                int epsilon_found = 0; // Indicates if the following non terminal has an epsilon transition
                                int t_or_nt_enum = VectorInt.at(g->rules[grammarID].rhs[a], currInd); // Indicates the token at currInd

                                if (t_or_nt_enum < TK_COUNT) { 
                                    
                                    /*
                                        If it is a terminal, push it into the Follow Set and break out of the loop.
                                    */
                                    
                                    Set.insert(st[index], t_or_nt_enum);
                                    break;
                                }

                                for (int k=0; k < Set.size(first[t_or_nt_enum - TK_COUNT]); k++) {

                                    /*
                                        For all elements in the First set of the following non terminal, we add them to the 
                                        follow of the original Non terminal, except if it is epsilon
                                    */

                                    if (Set.at(first[t_or_nt_enum - TK_COUNT], k) != TK_EPSILON)
                                        Set.insert(st[index], Set.at(first[t_or_nt_enum - TK_COUNT], k)); // Condition when the element is not epsilon
                                    
                                    else
                                        epsilon_found++; // Condition when the element is epsilon
                                    
                                }

                                if (!epsilon_found)
                                    break;  // If no epsilons have been found, we are done with our computation, and can break out of the loop
                                else 
                                    currInd++; // If there are epsilons in the First set, we will have to check the token after the token at currInd
                                

                                if (currInd >= VectorInt.size(g->rules[grammarID].rhs[a])) {

                                    /*
                                        If we are at the end of the production rule, say A ===> BCDE, and we want to find the 
                                        follow of E, then all elements in Follow(A) belong to Follow(E)

                                        However, if a production like A ===> BCA exists, we would be in a dilemma if we use the above
                                        logic, and so don't follow it
                                        
                                    */


                                    if(strcmp(g->rules[grammarID].lhs, t_or_nt_string(g, NT))){ 

                                        /*
                                            Execute code in this if condition if the transition is not of the form A ===> BCA
                                        */

                                        struct set* temp = follow_helper(g, st, grammarID, proc, first);

                                        for (int k = 0; k < Set.size(temp); k++)
                                            Set.insert(st[index], Set.at(temp, k));
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    return st[index];
}

/**
 * @brief Used to generate the Follow Sets
 * 
 * @param g The Grammar Rules
 * @param first An array of sets, which are the First sets 
 * @return struct set** An array of sets, which is the Follow Sets
 */
struct set** generate_follow(struct grammar* g, struct set** first) {
    struct set** follow = malloc(sizeof(struct set*) * g->rule_count);
    for(int i=0; i<g->rule_count; i++){
        follow[i] = NULL;
    }

    struct set** proc = follow_preprocess(g);

    follow[0] = Set.new();
    Set.insert(follow[0], -1);

     for(int i=1; i<g->rule_count; i++){
        if(!follow[i]){
            follow_helper(g, follow, i, proc, first);
        }
     }

    // freeing
    for (int i = 0; i < g->rule_count; i++)
        Set.free(&proc[i]);
    free(proc);

    return follow;
}


/**
 * @brief Prints the First and Follow Sets
 * 
 * @param g The Grammar Rules
 * @param first The First Set (An Array of Sets)
 * @param follow The Follow Sets (An Array of Sets)
 */
void print_first_follow(struct grammar *g, struct set **first, struct set **follow) {
    for (int i = 0; i < g->rule_count; i++) {
        printf("%s:\n", t_or_nt_string(g, i + TK_COUNT));

        printf("%10s", "first: ");
        for (int j = 0; j < Set.size(first[i]); j++)
            printf("%s ", t_or_nt_string(g, Set.at(first[i], j)));
        printf("\n");

        printf("%10s", "follow: ");
        for (int j = 0; j < Set.size(follow[i]); j++)
            printf("%s ", t_or_nt_string(g, Set.at(follow[i], j)));
        printf("\n\n");
    }

    return;
}

/**
 * @brief Free the memory allocated to the first/follow sets
 * 
 * @param p_first_or_follow Pointer to the pointer to the memory allocated to the set
 * @param g The grammar rule
 */
void free_first_and_follow(struct set ***p_first_or_follow, struct grammar *g) {
    assert(p_first_or_follow != NULL);
    assert(*p_first_or_follow != NULL);

    struct set **first_or_follow = *p_first_or_follow;

    for (int i = 0; i < g->rule_count; i++) {
        Set.free(&first_or_follow[i]);
    }
    free(first_or_follow);

    *p_first_or_follow = NULL;

    return;
}

/**
 * @brief Generate the Parse Table
 * 
 * @param g The grammar rules
 * @param first The First set
 * @param follow The follow Set
 * @return struct vector_int*** The Parse table 
 */
struct vector_int ***createParseTable(struct grammar *g, struct set **first, struct set **follow) {
    struct vector_int ***ret = calloc(g->rule_count, sizeof(*ret));

    for (int i = 0; i < g->rule_count; i++)
        ret[i] = calloc(TK_COUNT, sizeof(**ret)); // The entry corresponding to TK_EPSILON will contain `$`

    for (int i = 0; i < g->rule_count; i++) {
        struct grammar_rule rule = g->rules[i];

        for (int j = 0; j < rule.rhs_count; j++) {
            struct vector_int *rhs_tokens = rule.rhs[j];

            int k;
            for (k = 0; k < VectorInt.size(rhs_tokens); k++) {
                int t_or_nt_enum = VectorInt.at(rhs_tokens, k);

                if (t_or_nt_enum < TK_EPSILON) {
                    assert(ret[i][t_or_nt_enum] == NULL);
                    ret[i][t_or_nt_enum] = rhs_tokens;
                    break;
                }

                if (t_or_nt_enum == TK_EPSILON) {
                    // follow logic
                    struct set *follow_set = follow[i];

                    for (int l = 0; l < Set.size(follow_set); l++) {
                        int t = Set.at(follow_set, l);
                        t = t == -1 ? TK_EPSILON : t; // The entry corresponding to TK_EPSILON will contain `$`
                        assert(ret[i][t] == NULL);
                        ret[i][t] = rhs_tokens;
                    }

                    break;
                }

                struct set *first_set = first[VectorInt.at(rhs_tokens, k) - TK_COUNT];
                int contains_epsilon = Set.search(first_set, TK_EPSILON);

                for (int l = 0; l < Set.size(first_set); l++) {
                    int t = Set.at(first_set, l);
                    if (t != TK_EPSILON) {
                        assert(ret[i][t] == NULL);
                        ret[i][t] = rhs_tokens;
                    }
                }

                if (!contains_epsilon)
                    break;
            }

            if (k == VectorInt.size(rhs_tokens)) {
                // follow logic
                struct set *follow_set = follow[i];

                for (int l = 0; l < Set.size(follow_set); l++) {
                    int t = Set.at(follow_set, l);
                    t = t == -1 ? TK_EPSILON : t; // The entry corresponding to TK_EPSILON will contain `$`
                    assert(ret[i][t] == NULL);
                    ret[i][t] = rhs_tokens;
                }

            }
        }
    }

    return ret;
}

/**
 * @brief Used to print the parse table
 * 
 * @param g The grammar
 * @param parse_table The parse table
 */
void print_parse_table(struct grammar *g, struct vector_int ***parse_table) {
    int count = 0;
    for (int i = 0; i < g->rule_count; i++) {
        for (int j = 0; j < TK_COUNT; j++) {
            if (parse_table[i][j] != NULL) {
                count++;
                printf("%d. Stack symbol: %s, next token: %s\n", count, t_or_nt_string(g, i + TK_COUNT), t_or_nt_string(g, j));
                printf("Production rule: %s ===> ", t_or_nt_string(g, i + TK_COUNT));

                for (int k = 0; k < VectorInt.size(parse_table[i][j]); k++)
                    printf("%s ", t_or_nt_string(g, VectorInt.at(parse_table[i][j], k)));

                printf("\n\n");
            }
        }
    }
    printf("Total parse table entries: %d\n", count);

    return;
}

/**
 * @brief Used to free the memory allocated to the parse table
 * 
 * @param p_parse_table Pointer to the pointer to the memory allocated to the parse table
 * @param g The grammar
 */
void free_parse_table(struct vector_int ****p_parse_table, struct grammar *g) {
    assert(p_parse_table != NULL);
    assert(*p_parse_table != NULL);

    struct vector_int ***parse_table = *p_parse_table;

    for (int i = 0; i < g->rule_count; i++)
        free(parse_table[i]);

    free(parse_table);
    *p_parse_table = NULL;

    return;
}

/**
 * @brief Used to parse in the file
 * 
 * @param file_name Path to the file
 * @param g The grammar
 * @param st The symbol table
 * @return struct tree_node* The parse tree generated from parsing the file
 */
struct tree_node *parse(char *file_name, struct grammar *g, struct symbol_table *st) {
    struct set **first = generate_first(g);
    struct set **follow = generate_follow(g, first);
    struct vector_int ***parse_table = createParseTable(g, first, follow);

    struct stack *parse_stack = Stack.new();
    Stack.push(parse_stack, -1); // `$`, end-of-file marker
    Stack.push(parse_stack, TK_COUNT);

    struct stack *tree_indices = Stack.new();
    Stack.push(tree_indices, -1);
    int current_tree_index = 0;

    IOHandler *io = createIOHandler(file_name);

    struct tree_node *root = Tree.new(TK_COUNT);
    struct tree_node *tracker = root;

    while (!io->inputFin) {
        TOKEN *tok = getNextToken(io, st);
        int err_reported_flag = 0;

        if (tok == NULL || tok->data->token_type == TK_COMMENT)
            continue;

        if (Stack.top(parse_stack) == -1 && (int) tok->data->token_type != -1) {
            if (!err_reported_flag++) {
                /*
                    Used in the case when the stack has been emptied (The parsing has been completed), but the input has not been completed
                */
                parser_error("Line %4d: Unexpected token `%s`; stack configuration is empty!\n", tok->lineNumber, tok->data->stringLexeme);
            } 
                
            continue;
        }
        
        while ((int) tok->data->token_type != Stack.top(parse_stack)) {
            int nt = Stack.pop(parse_stack);

            if (nt == -1 && (int) tok->data->token_type != -1) {
                if (!err_reported_flag++) {
                    /*
                        Used in the case when the stack has been emptied (The parsing has been completed), but the input has not been completed
                    */
                    parser_error("Line %4d: Unexpected token `%s`; stack configuration is empty!\n", tok->lineNumber, tok->data->stringLexeme);
                }
                Stack.push(parse_stack, -1);
                break;
            }

            if (nt == TK_EPSILON)
                continue; // pop stack (syn)

            if (nt < TK_COUNT) {
                if (!err_reported_flag++) {
                    if ((int) tok->data->token_type != -1)
                        parser_error("Line %4d: Unexpected token type %s with value `%s`; (expected %s)\n", tok->lineNumber, t_or_nt_string(g, tok->data->token_type), tok->data->stringLexeme, t_or_nt_string(g, nt)); // this is a non-terminal, should print it out better
                    else
                        parser_error("End of token stream, but expected `%s`\n", t_or_nt_string(g, nt));
                }

                continue; // pop stack (syn), and continue derivation for token
            }

            struct vector_int *rhs = parse_table[nt - TK_COUNT][(int) tok->data->token_type == -1 ? TK_EPSILON : tok->data->token_type];

            if (rhs == NULL) {
                // EPS transitions
                if (Set.search(first[nt - TK_COUNT], TK_EPSILON)) {
                    // maybe comment out this error?
                    if (!err_reported_flag++)
                        parser_error("Line %4d: Unexpected token type %s with value `%s`; stack symbol `%s` undergoing epsilon transition\n", tok->lineNumber, t_or_nt_string(g, tok->data->token_type), tok->data->stringLexeme, t_or_nt_string(g, nt));
                    continue; // pop stack (syn)
                }

                if ((int) tok->data->token_type != -1) {
                    if (!err_reported_flag++)
                        parser_error("Line %4d: Unexpected token type %s with value `%s`; (expected %s)\n", tok->lineNumber, t_or_nt_string(g, tok->data->token_type), tok->data->stringLexeme, t_or_nt_string(g, nt));
                } else {
                    if (!err_reported_flag++)
                        parser_error("End of token stream, but stack symbol `%s` present\n", t_or_nt_string(g, nt));
                    continue; // pop stack
                }

                if (Set.search(follow[nt - TK_COUNT], tok->data->token_type) || IS_DELIM(tok->data->token_type)) {
                    continue; // pop stack (syn), and continue derivation for token
                } else {
                    Stack.push(parse_stack, nt); // re-insert in parsing stack
                    break; // skip token (err)
                }
            }

            for (int i = VectorInt.size(rhs) - 1; i >= 0; i--)
                Stack.push(parse_stack, VectorInt.at(rhs, i));

            if (NO_ERRORS && current_tree_index != -1) {
                // tree.insert nt, rhs
                assert(tracker->data == nt);
                Tree.insert_children(tracker, rhs);

                // go to child
                assert(current_tree_index == 0);

                while (1) {
                    assert(tracker->children_count);

                    if (current_tree_index != tracker->children_count) {
                        Stack.push(tree_indices, current_tree_index + 1);
                        tracker = tracker->children[current_tree_index];
                        current_tree_index = 0;
                    } else {
                        tracker = tracker->parent;
                        current_tree_index = Stack.pop(tree_indices);

                        if (tracker == NULL)
                            break;

                        continue;
                    }

                    if (tracker->data == TK_EPSILON) {
                        tracker = tracker->parent;
                        current_tree_index = Stack.pop(tree_indices);

                        if (tracker == NULL)
                            break;

                        continue;
                    } else 
                        break;
                }
            }
        }

        if ((int) tok->data->token_type == Stack.top(parse_stack)) {
            Stack.pop(parse_stack);

            // next leaf node
            if (NO_ERRORS && current_tree_index != -1) {
                tracker->line_number = tok->lineNumber;
                tracker->lex_data = tok->data->lexeme;
                tracker->lexeme = calloc(strlen(tok->data->stringLexeme) + 1, sizeof(char));
                strcpy(tracker->lexeme, tok->data->stringLexeme);

                while (1) {
                    assert(tracker->children_count == 0);

                    tracker = tracker->parent;
                    current_tree_index = Stack.pop(tree_indices);

                    if (tracker == NULL)
                        break;

                    while (current_tree_index == tracker->children_count) {
                        tracker = tracker->parent;
                        current_tree_index = Stack.pop(tree_indices);

                        if (tracker == NULL)
                            break;
                    }

                    if (tracker == NULL)
                        break;

                    Stack.push(tree_indices, current_tree_index + 1);
                    tracker = tracker->children[current_tree_index];
                    current_tree_index = 0;

                    if (tracker->data != TK_EPSILON)
                        break;
                }
            }
        }

        free(tok);
    }

    if (NO_ERRORS) {
        assert(Stack.is_empty(parse_stack)); // should pretty much always be true, remove this later if not necessary
        printf("Parsing of %s completed successfully! Input source code is syntactically correct!\n", file_name);
    } else {
        Tree.free(&root);
    }

    Stack.free(&parse_stack);
    Stack.free(&tree_indices);
    closeHandler(io);
    free_first_and_follow(&first, g);
    free_first_and_follow(&follow, g);

    free_parse_table(&parse_table, g);

    return root;
}


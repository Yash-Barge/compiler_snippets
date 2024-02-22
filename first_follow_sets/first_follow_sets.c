#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_follow_sets.h"
#include "set.h"
#include "vector.h"

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
    int offset = 0;

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

struct grammar_rule {
    // `lhs` is the string `"ADS"`, if the production rule is `<ADS> ===> <B> | <C>`
    char *lhs;
    // Number of different productions `<A> ===> <B> | <C>` gives `2`; `<A> ===> <B> | <C> | <D>` gives `3`.
    int rhs_count;
    // Array of `vector<int>`. Each vector encodes a production rule.
    //
    // `<A> ===> <B> <C> | <D> <E> <F> | <G> | <H> TK_ONE TK_TWO <I>` will be encoded in `4` different `vectors`
    // `rhs[0]` will contain encoding `<B> <C>` (`2 ints`)
    // `rhs[1]` will contain encoding `<D> <E> <F>` (`3 ints`)
    // `rhs[2]` will contain encoding `<G>` (`1 int`)
    // `rhs[3]` will contain encoding `<H> TK_ONE TK_TWO <I>` (`4 ints`)
    struct vector_int **rhs;
};

struct grammar {
    // The number of non-terminals in the grammar. It is also the number of production rules if `<A> ===> <B> | <C>` is counted as a single production rule.
    int rule_count;
    // An array of `grammar_rule`s; each production rule of the form `<A> ===> <B> | <C>` is encoded in a single index.
    struct grammar_rule rules[];
};

/**
 * @brief Helper function for `make_grammar()`. Should not be used outside of it.
 * @param raw_prod_rule String production rule in the form of `<A> ===> <B> <C> TK_EXAMPLE ...`
 * @param index Pointer to index, starts locating next terminal from that index
 * @return 
 */
char *get_next_t_or_nt(const char *restrict const raw_prod_rule, int *index) {
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
    exit(1);
}

/** 
 * - Makes grammar by opening the file specified in `grammar_file_path`. Grammar rules should be of the form `<A> ===> <B> <C> TK_EXAMPLE ...`.

 * - Each non-terminal should appear on the left-hand side of a production rule only once. Multiple production rules should be separated by the `|`
 * character. E.g. `<A> ===> <B> TK_P | <C> TK_Q | EPS`.
 * 
 * - Epsilon should be denoted by `EPS`.
 * 
 * - Non-terminals should be enclosed by a pair of angular brackets `<>`. Terminals should start with `TK_...` and should be composed of only capital
 * alphabet letters.
 * 
 * - The left-hand side of the first grammar rule in the file is taken as the starting non-terminal.
 * 
 * @param grammar_file_path File path to a file, encoded with the grammar in plain ASCII. E.g. `grammar_folder/grammar.txt`.
 * @return A `struct grammar` pointer; fields of that can be found explained above the struct definition.
 */
struct grammar *make_grammar(const char *restrict const grammar_file_path) {
    FILE *grammar_file = fopen(grammar_file_path, "r");

    struct vector_string *prod_rules_raw = VectorString.new();

    // TODO: Redo this properly
    char buffer[MAXSIZE];
    while (fgets(buffer, sizeof(buffer), grammar_file)) {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        if (strlen(buffer) && substring_count(buffer, "===>")) // not empty and grammar rule
            VectorString.push_back(prod_rules_raw, buffer);
    }

    struct grammar *ret = malloc(sizeof(*ret) + sizeof(struct grammar_rule) * VectorString.size(prod_rules_raw));
    ret->rule_count = VectorString.size(prod_rules_raw);

    // Initializes only lhs and rhs_count
    for (int i = 0; i < ret->rule_count; i++) {
        const char *const temp = VectorString.at(prod_rules_raw, i);

        int end_of_lhs = 0;
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
        while (index < strlen(temp)) {
            char *t_or_nt = get_next_t_or_nt(temp, &index);

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
 * @brief Stringifies the enumified (non-)terminal.
 * @param g Grammar.
 * @param enumified_t_or_nt Found in `g->rules[index].rhs[prod_rule_num][t_or_nt_num]`.
 * @return Human-readable string of (non-)terminal. Do not free this string.
 */
const char *const t_or_nt_string(struct grammar *g, int enumified_t_or_nt) {
    return enumified_t_or_nt < TK_COUNT ? TOK_STRING[enumified_t_or_nt] : g->rules[enumified_t_or_nt - TK_COUNT].lhs;
}

int main(void){
    struct grammar *g = make_grammar("grammar.txt");

    // For testing make_grammar(), can be removed.
    for (int i = 0; i < g->rule_count; i++) {
        printf("NT: '%s', enumified: %d\n", g->rules[i].lhs, TK_COUNT + i);
        printf("RHS:\n");

        for (int j = 0; j < g->rules[i].rhs_count; j++) {
            for (int k = 0; k < VectorInt.size(g->rules[i].rhs[j]); k++) {
                int enumified_tok = VectorInt.at(g->rules[i].rhs[j], k);
                printf("%s ", t_or_nt_string(g, enumified_tok));
            }
            printf("\n");
        }

        printf("NT: '%s' done\n\n", g->rules[i].lhs);
    }

    return 0;
}

#pragma once

#include "vector.h"
#include "set.h"

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
struct grammar *make_grammar(const char *restrict const grammar_file_path);

/**
 * @brief Stringifies the enumified (non-)terminal.
 * @param g Grammar.
 * @param enumified_t_or_nt Found in `g->rules[index].rhs[prod_rule_num][t_or_nt_num]`.
 * @return Human-readable string of (non-)terminal. Do not free this string.
 */
const char *t_or_nt_string(struct grammar *g, int enumified_t_or_nt);

/**
 * @brief 
 * 
 * @param g The grammar rules generated above 
 * @return struct set** 
 */
struct set **generate_first(struct grammar *g);

/**
 * @brief 
 * 
 * @param g The grammar rules generated above 
 * @return struct set** 
 */
struct set **generate_follow(struct grammar *g, struct set **first);

void free_grammar(struct grammar **p_g);

#include "parser.h"

// TODO: replace all asserts in this function with proper error handling
struct vector_int ***make_parse_table(struct grammar *g, struct set **first, struct set **follow) {
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

#include "parser.h"
#include "dfa.h"
#include "stack.h"

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

struct tree_node *parse(char *file_name, struct grammar *g) {
    struct set **first = generate_first(g);
    struct set **follow = generate_follow(g, first);
    struct vector_int ***parse_table = make_parse_table(g, first, follow);
    free_first_and_follow(&first, g);
    free_first_and_follow(&follow, g);

    struct stack *parse_stack = Stack.new();
    Stack.push(parse_stack, -1); // `$`, end-of-file marker
    Stack.push(parse_stack, TK_COUNT);

    // TODO: lexer vs parser errors need to be re-thought
    IOHandler *io = createIOHandler(file_name);

    struct symbol_table *st = SymbolTable.init();

    struct tree_node *root = Tree.new(TK_COUNT);
    struct tree_node *tracker = root;

    while (!io->inputFin) {
        TOKEN *tok = runDFA(io, st);

        if (tok == NULL)
            continue;
        

        while (tok->data->token_type != Stack.top(parse_stack)) {
            int nt = Stack.pop(parse_stack);

            if (nt == TK_EPSILON)
                continue;

            if (nt < TK_COUNT) {
                // TODO:
                if (tok->data->token_type == TK_NUM)
                    ;
                else if (tok->data->token_type == TK_RNUM)
                    ;
                else
                    fprintf(stderr, "\033[1;31merror: \033[0mUnexpected token `%s` at line %d (expected %s)\n", tok->data->lexeme.lexeme, tok->lineNumber, t_or_nt_string(g, nt)); // this is a non-terminal, should print it out better

                exit(1);
            }

            struct vector_int *rhs = parse_table[nt - TK_COUNT][tok->data->token_type];

            if (rhs == NULL) {
                // TODO:
                if (tok->data->token_type == TK_NUM)
                    ;
                else if (tok->data->token_type == TK_RNUM)
                    ;
                else
                    fprintf(stderr, "\033[1;31merror: \033[0mUnexpected token `%s` at line %d\n(non-terminal %s; no parse table entry)\n", tok->data->lexeme.lexeme, tok->lineNumber, t_or_nt_string(g, nt));

                exit(1);
            }

            for (int i = VectorInt.size(rhs) - 1; i >= 0; i--)
                Stack.push(parse_stack, VectorInt.at(rhs, i));
            
            // tree.insert nt, rhs
            assert(tracker->data == nt); // data in it IS nt
            Tree.insert(tracker, rhs);

            // in-order traversal to next non-terminal
            while (1) {
                int exit_loop = 0;

                for (int i = 0; i < tracker->children_count; i++) {
                    if (tracker->children[i].children_count == 0 && tracker->children[i].data > TK_COUNT) {
                        tracker = &(tracker->children[i]);
                        exit_loop = 1;
                        break;
                    }
                }

                if (exit_loop)
                    break;
                
                tracker = tracker->parent;

                if (tracker == NULL) // I'm guessing this is the end of parsing?
                    break;
            }
        }

        assert(tok->data->token_type == Stack.top(parse_stack)); // should pretty much always be true, remove this later if not necessary
        Stack.pop(parse_stack);

        free(tok);
    }

    assert(Stack.is_empty(parse_stack)); // should pretty much always be true, remove this later if not necessary
    printf("Parsing of %s completed successfully!\n", file_name);

    closeHandler(io);
    free_parse_table(&parse_table, g);

    return root;
}

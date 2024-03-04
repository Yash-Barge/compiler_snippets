#include "parser.h"
#include "dfa.h"
#include "stack.h"
#include "errors.h"
#include "symbol_table.h" //temp

#define NO_ERRORS (!get_lexer_error_count() && !get_parser_error_count())

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

struct tree_node *parse(char *file_name, struct grammar *g, struct symbol_table *st) {
    struct set **first = generate_first(g);
    struct set **follow = generate_follow(g, first);
    struct vector_int ***parse_table = make_parse_table(g, first, follow);

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
        TOKEN *tok = runDFA(io, st);

        if (tok == NULL || tok->data->token_type == TK_COMMENT)
            continue;

        if (Stack.top(parse_stack) == -1 && (int) tok->data->token_type != -1) {
            parser_error("Line %4d: Unexpected token `%s`; stack configuration is empty!\n", tok->lineNumber, tok->data->stringLexeme);
            continue;
        }
        
        while ((int) tok->data->token_type != Stack.top(parse_stack)) {
            int nt = Stack.pop(parse_stack);

            if (nt == -1 && (int) tok->data->token_type != -1) {
                parser_error("Line %4d: Unexpected token `%s`; stack configuration is empty!\n", tok->lineNumber, tok->data->stringLexeme);
                Stack.push(parse_stack, -1);
                break;
            }

            if (nt == TK_EPSILON)
                continue; // pop stack (syn)

            if (nt < TK_COUNT) {
                if ((int) tok->data->token_type != -1)
                    parser_error("Line %4d: Unexpected token type %s with value `%s`; (expected %s)\n", tok->lineNumber, t_or_nt_string(g, tok->data->token_type), tok->data->stringLexeme, t_or_nt_string(g, nt)); // this is a non-terminal, should print it out better
                else
                    parser_error("Error: End of token stream, but expected `%s`\n", t_or_nt_string(g, nt));

                continue; // pop stack (syn), and continue derivation for token
            }

            struct vector_int *rhs = parse_table[nt - TK_COUNT][(int) tok->data->token_type == -1 ? TK_EPSILON : tok->data->token_type];

            if (rhs == NULL) {
                // EPS transitions
                if (Set.search(first[nt - TK_COUNT], TK_EPSILON)) {
                    // maybe comment out this error?
                    parser_error("Line %4d: Unexpected token type %s with value `%s`; stack symbol `%s` undergoing epsilon transition\n", tok->lineNumber, t_or_nt_string(g, tok->data->token_type), tok->data->stringLexeme, t_or_nt_string(g, nt));
                    continue; // pop stack (syn)
                }

                if ((int) tok->data->token_type != -1)
                    parser_error("Line %4d: Unexpected token type %s with value `%s`; (expected %s)\n", tok->lineNumber, t_or_nt_string(g, tok->data->token_type), tok->data->stringLexeme, t_or_nt_string(g, nt));
                else {
                    parser_error("Error: End of token stream, but stack symbol `%s` present\n", t_or_nt_string(g, nt));
                    continue; // pop stack
                }

                if (Set.search(follow[nt - TK_COUNT], tok->data->token_type)) {
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
        printf("Parsing of %s completed successfully!\n", file_name);
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

#include <stdio.h>
#include <stdlib.h>

#include "dfa.h"
#include "symbol_table.h"
#include "errors.h"
#include "first_follow_sets.h"
#include "parser.h"
#include "tree.h"

void print_source_without_comments(char *file_name) {
    IOHandler *io = createIOHandler(file_name);

    while (!io->inputFin) {
        char c = getChar(io, true);

        if (c == '%')
            while (c != '\n' && c != '\0')
                c = getChar(io, true);

        printf("%c", c);
    }

    printf("\n");

    closeHandler(io);

    return;
}

void lexer(char *file_name) {
    IOHandler *io = createIOHandler(file_name);

    struct symbol_table *st = SymbolTable.init();

    while (!io->inputFin) {
        TOKEN *tok = runDFA(io, st);
        if (tok != NULL) {
            printToken(tok);
            free(tok);
        }
    }

    closeHandler(io);

    if (get_error_count())
        fprintf(stderr, "\033[1;31merror: \033[0mLexing of file %s failed with \033[1;31m%d lexer error(s)\033[0m\n", file_name, get_error_count());
    else
        printf("Lexing of file %s successful!\n", file_name);
    
    reset_error_count();

    return;
}

void pre_order_print(struct grammar *g, struct tree_node *t) {
    printf("\n%s", t_or_nt_string(g, t->data));

    struct tree_node *tracker = t->parent;

    while (tracker) {
        printf(" <=== %s", t_or_nt_string(g, tracker->data));
        tracker = tracker->parent;
    }
    printf("\n");

    if (t->children_count) {
        printf("number of children, %d: ", t->children_count);
        for (int i = 0; i < t->children_count; i++)
            printf("%s ", t_or_nt_string(g, t->children[i].data));
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < t->children_count; i++)
        pre_order_print(g, &(t->children[i]));

    if (t->children_count) {
        printf("done: %s", t_or_nt_string(g, t->data));

        struct tree_node *tracker = t->parent;

        while (tracker) {
            printf(" <=== %s", t_or_nt_string(g, tracker->data));
            tracker = tracker->parent;
        }
        printf("\n");
    }

    return;
}

void parser(char *file_name) {    
    struct grammar *g = make_grammar("nalanda_grammar.txt");
    struct set **first = generate_first(g);
    struct set **follow = generate_follow(g, first);

    // print_first_follow(g, first, follow);

    struct vector_int ***parse_table = make_parse_table(g, first, follow);

    // print_parse_table(g, parse_table);

    struct tree_node *tree = parse(file_name, g);
    pre_order_print(g, tree);

    Tree.free(&tree);
    free_first_and_follow(&first, g);
    free_first_and_follow(&follow, g);
    free_parse_table(&parse_table, g);
    free_grammar(&g);

    return;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { // should eventually change to 3, to put parser output in a file
        error("Invalid number of arguments! Execute program followed by source file!\n");
        return 1;
    }

    int input = -1;

    while (input != 0) {
        printf("Enter a number from 0-4: ");
        scanf("%d", &input);

        switch (input) {
        case 0:
            printf("Exiting program...\n");
            break;
        case 1:
            print_source_without_comments(argv[1]);
            break;
        case 2:
            lexer(argv[1]);
            break;
        case 3:
            parser(argv[1]);
            break;
        case 4:
            printf("TODO...\n");
            break;
        default:
            printf("Unrecognized input %d!\n", input);
            continue;
        }

    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    SymbolTable.free(&st);

    if (get_lexer_error_count())
        fprintf(stderr, "\033[1;31merror: \033[0mLexing of file %s failed with \033[1;31m%d lexer error(s)\033[0m\n", file_name, get_lexer_error_count());
    else
        printf("Lexing of file %s successful!\n", file_name);
    
    reset_error_count();

    return;
}

void in_order_print(struct grammar *g, struct tree_node *t, char* file_name) {
    if(t == NULL) return;
    in_order_print(g, &(t->children[0]), file_name);
    FILE *fp = fopen(file_name, "a");
    printf("%s\n", t_or_nt_string(g, t->data));
    fprintf(fp, "%s\n", t_or_nt_string(g, t->data));
    fclose(fp);
    for(int i=1; i<t->children_count; i++) {
        in_order_print(g, &(t->children[i]), file_name);
    }
}


// TODO: remove the first/follow, parse_table stuff from here for final submission
void parser(char *file_name) {    
    struct grammar *g = make_grammar("nalanda_grammar.txt");
    struct set **first = generate_first(g);
    struct set **follow = generate_follow(g, first);

    // print_first_follow(g, first, follow);

    struct vector_int ***parse_table = make_parse_table(g, first, follow);

    // print_parse_table(g, parse_table);

    struct symbol_table *st = SymbolTable.init();
    struct tree_node *tree = parse(file_name, g, st);

    // if (get_lexer_error_count()) {
    //     fprintf(stderr, "\033[1;31merror: \033[0mParsing of file %s was aborted due to \033[1;31m%d lexer error(s)\033[0m\n", file_name, get_lexer_error_count());
    //     if (get_parser_error_count())
    //         fprintf(stderr, "\033[1;31merror: %d parser error(s)\033[0m detected before parsing was aborted\n", get_parser_error_count());
    // }
    // else if (get_parser_error_count()) {
    //     fprintf(stderr, "\033[1;31merror: \033[0mParsing of file %s was failed due to \033[1;31m%d parser error(s)\033[0m\n", file_name, get_parser_error_count());
    // } else
    //     pre_order_print(g, tree);

    if (get_lexer_error_count() || get_parser_error_count())
        fprintf(stderr, "\033[1;31merror: \033[0mParsing failed with \033[1;31m%d lexer error(s)\033[0m and \033[1;31m%d parser error(s)\033[0m\n", get_lexer_error_count(), get_parser_error_count());
    else
        in_order_print(g, tree, "parse_tree.txt");

    reset_error_count();

    if (tree)
        Tree.free(&tree);

    free_first_and_follow(&first, g);
    free_first_and_follow(&follow, g);
    free_parse_table(&parse_table, g);
    SymbolTable.free(&st);
    free_grammar(&g);

    return;
}

void parser_timer(char* file_name) {
    clock_t start, end;
    double time;
    start = clock();
    parser(file_name);
    end = clock();
    time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime taken to complete execution = %lf\n", time);
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
            parser_timer(argv[1]);
            break;
        
        default:
            printf("Unrecognized input %d!\n", input);
            continue;
        }

    }

    return 0;
}

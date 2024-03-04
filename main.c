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

void in_order_print(struct grammar *g, struct tree_node *t, FILE* fp) {
    const char dummy[] = "----------------------------------------------------------------";

    if (t == NULL)
        return;

    if (t->children_count)
        in_order_print(g, t->children[0], fp);

    if (t->data < TK_EPSILON) {
        fprintf(fp, "%32.32s %6d %26s", t->lexeme, t->line_number, t_or_nt_string(g, t->data));

        if(t->data == TK_NUM) {
            fprintf(fp, "%22lld ", t->lex_data.intVal);
        }
        else if(t->data == TK_RNUM) {
            fprintf(fp, "%22lf ", t->lex_data.floatVal);
        }
        else {
            fprintf(fp, " %21.15s ", dummy);
        }

        fprintf(fp, "%10s %20s\n", "Yes", t->parent ? t_or_nt_string(g, t->parent->data) : "NULL");

    } else {
        fprintf(fp, "%32.15s %6.3s %26s %21.15s %10.8s %20.14s\n", dummy, dummy, t_or_nt_string(g, t->data), dummy, "No", dummy);
    }

    for (int i = 1; i < t->children_count; i++) {
        in_order_print(g, t->children[i], fp);
    }
}

void print_tree(struct grammar *g, struct tree_node *root, char *file_path) {
    FILE* fp = fopen(file_path, "w+");
    fclose(fp);
    fp = fopen(file_path, "r+");

    fprintf(fp, "%32.15s %6.5s %26s %21.15s %10.8s %20.14s\n", "Lexeme", "Line", "Token Type", "Value", "IsLeaf", "Parent Token");
    printf("%32.15s %6.5s %26s %21.15s %10.8s %20.14s\n", "Lexeme", "Line", "Token Type", "Value", "IsLeaf", "Parent Token");

    in_order_print(g, root, fp);
    fclose(fp);

    return;
}

// TODO: remove the first/follow, parse_table stuff from here for final submission
void parser(char *file_name, char *parse_tree_file) {    
    struct grammar *g = make_grammar("nalanda_grammar.txt");

    struct symbol_table *st = SymbolTable.init();
    struct tree_node *tree = parse(file_name, g, st);

    if (get_lexer_error_count() || get_parser_error_count())
        fprintf(stderr, "\033[1;31merror: \033[0mParsing failed with \033[1;31m%d lexer error(s)\033[0m and \033[1;31m%d parser error(s)\033[0m\n", get_lexer_error_count(), get_parser_error_count());
    else
        print_tree(g, tree, parse_tree_file);

    reset_error_count();

    if (tree)
        Tree.free(&tree);

    SymbolTable.free(&st);
    free_grammar(&g);

    return;
}

void parser_timer(char* file_name, char *parse_file_name) {
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();
    parser(file_name, parse_file_name);
    end_time = clock();
    total_CPU_time = (double) (end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
    printf("Total CPU Time = %lf\n", total_CPU_time);
    printf("Total CPU Time in Seconds= %lf\n", total_CPU_time_in_seconds);
}

int main(int argc, char *argv[]) {
    if (argc != 3) { // should eventually change to 3, to put parser output in a file
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
            parser(argv[1], argv[2]);
            break;
        case 4:
            parser_timer(argv[1], argv[2]);
            break;
        
        default:
            printf("Unrecognized input %d!\n", input);
            continue;
        }

    }

    return 0;
}

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
#include <time.h>

#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"
#include "parser.h"
#include "others.h"

void lexer(char *file_name) {
    IOHandler *io = createIOHandler(file_name);

    struct symbol_table *st = SymbolTable.init();
    printf("Line Number\tToken\t\t\t\tLexeme\n");
    while (!io->inputFin) {
        TOKEN *tok = getNextToken(io, st);
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
        printf("\n\033[1;32msuccess:\033[0m Lexing of file %s successful!\n", file_name);
    
    reset_error_count();

    return;
}

void in_order_print(struct grammar *g, struct tree_node *t, FILE* fp) {
    const char dummy[] = "----------------------------------------------------------------";

    if (t == NULL)
        return;

    if (t->children_count)
        in_order_print(g, t->children[0], fp);

    if (t->data == TK_EPSILON) {
        fprintf(fp, "%32.15s %6.3s %26.14s %21.15s %30.25s %10.8s %30.25s\n", dummy, dummy, t_or_nt_string(g, t->data), dummy, t_or_nt_string(g, t->parent->data), "Yes", dummy);
    } else if (t->data >= TK_COUNT && t->children_count == 0) { // two error checkers
        fprintf(fp, "\t\t\tParsing error on line number %d: %s could not be generated from token stream\n", t->line_number, t_or_nt_string(g, t->data));
    } else if (t->data < TK_EPSILON && t->lexeme == NULL) {
        fprintf(fp, "\t\t\tParsing error on line number %d: %s not in token stream\n", t->line_number, t_or_nt_string(g, t->data));
    } else if (t->data < TK_EPSILON) { // normal parse printing from this point
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

        fprintf(fp, "%30.25s %10.8s %30.25s\n", t->parent ? t_or_nt_string(g, t->parent->data) : "NULL", "Yes", dummy);

    } else if(t->parent){
        fprintf(fp, "%32.15s %6.3s %26.14s %21.15s %30.25s %10.8s %30.25s\n", dummy, dummy, dummy, dummy, t_or_nt_string(g, t->parent->data), "No", t_or_nt_string(g, t->data));
    }
    else {
        fprintf(fp, "%32.15s %6.3s %26.14s %21.15s %30.25s %10.8s %30.25s\n", dummy, dummy, dummy, dummy, "ROOT","No", t_or_nt_string(g, t->data));
    }

    for (int i = 1; i < t->children_count; i++) {
        in_order_print(g, t->children[i], fp);
    }
}

void printParseTree(struct grammar *g, struct tree_node *root, char *file_path) {
    FILE* fp = fopen(file_path, "w+");

    if (get_lexer_error_count() || get_parser_error_count())
        fprintf(fp, "NOTE: Parsing failed, output is partial parse tree\n\n");

    fprintf(fp, "%32.15s %6.5s %26s %21.15s %30.25s %10.8s %30.25s\n", "Lexeme", "Line", "Token Type", "Value", "Parent Node Symbol", "IsLeaf", "Node Symbol");

    in_order_print(g, root, fp);
    fclose(fp);

    return;
}

// TODO: remove the first/follow, parse_table stuff from here for final submission
void parseInputSourceCode(char *file_name, char *parse_tree_file) {    
    struct grammar *g = make_grammar("grammar.txt");

    struct symbol_table *st = SymbolTable.init();
    struct tree_node *tree = parse(file_name, g, st);

    if (get_lexer_error_count() || get_parser_error_count())
        fprintf(stderr, "\033[1;31merror: \033[0mParsing failed with \033[1;31m%d lexer error(s)\033[0m and \033[1;31m%d parser error(s)\033[0m\n", get_lexer_error_count(), get_parser_error_count());
    else
        printf("\n\033[1;32msuccess:\033[0m Parsing of %s completed successfully! Input source code is syntactically correct!\n", file_name);
    
    printParseTree(g, tree, parse_tree_file);

    reset_error_count();

    Tree.free(&tree);

    SymbolTable.free(&st);
    free_grammar(&g);

    return;
}

void parser_timer(char* file_name, char *parse_file_name) {
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();
    parseInputSourceCode(file_name, parse_file_name);
    end_time = clock();
    total_CPU_time = (double) (end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
    printf("\nTotal CPU Time (in microseconds) = %lf\n", total_CPU_time);
    printf("Total CPU Time (in seconds) = %lf\n", total_CPU_time_in_seconds);

    return;
}

int main(int argc, char *argv[]) {
    if (argc != 3) { 
        error("Invalid number of arguments! Execute program followed by source file!\n");
        return 1;
    }

    printf("\n\033[1;35mnote: \033[0mFor the Synchronization Sets, we have incorporated 3 major techniques:\n\t1. If the top of the stack is a non terminal, and it does not have the current token in it’s First set but has it in it’s Follow Set, then we pop the non terminal\n\t2. If case 1 fails, and the non terminal has an epsilon transition, we take the epsilon transition\n\t3. If both case 1 and 2 fail, then we discard the token.\n");

    printf("\n\033[1;33minfo: \033[0m FIRST and FOLLOW set automated\n");
    printf("\033[1;33minfo: \033[0m Both lexical and syntax analysis modules implemented\n");
    printf("\033[1;33minfo: \033[0m No known segmentation faults\n");
    printf("\033[1;33minfo: \033[0m Works with all testcases\n");
    printf("\033[1;33minfo: \033[0m Parse tree construction functional\n");

    printf("\n\033[1;32mstatus: \033[0m All requirements satisified and tested!\n");


    int input = -1;

    while (input != 0) {
        printf("\n\033[1;36mInput 0: \033[0mExit\n");
        printf("\033[1;36mInput 1: \033[0mPrint testcase without comments\n");
        printf("\033[1;36mInput 2: \033[0mPrint Testcase After Lexing Step\n");
        printf("\033[1;36mInput 3: \033[0mPrint testcase After Parsing\n");
        printf("\033[1;36mInput 4: \033[0mTime Testcase for Parsing\n");
        printf("Enter a number from 0-4: ");

        if (!scanf("%d", &input)) { // clear stdin, in case of string
            error("Unrecognized input `");
            char c;
            while ((c = getc(stdin)) != '\n' && c != EOF) 
                printf("%c", c);
            printf("`\n");
            continue;
        }

        switch (input) {
        case 0:
            printf("Exiting program...\n");
            break;
        case 1:
            removeComments(argv[1]);
            break;
        case 2:
            lexer(argv[1]);
            break;
        case 3:
            parseInputSourceCode(argv[1], argv[2]);
            break;
        case 4:
            parser_timer(argv[1], argv[2]);
            break;
        
        default:
            error("Unrecognized input `%d`\n", input);
            break;
        }

    }

    return 0;
}

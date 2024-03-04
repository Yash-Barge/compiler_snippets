// Warnings?

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

static int lexer_error_count = 0;
static int parser_error_count = 0;

/**
 * @brief Used to print a case of errors
 * 
 * @param fmt Error Message
 * @param ... 
 */
void error(const char *fmt, ...) {
    char *prefix = "\033[1;31merror: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    return;
}

/**
 * @brief Used to print a case of Lexical Errors
 * 
 * @param fmt Error Message
 * @param ... 
 */
void lexer_error(const char *fmt, ...) {
    lexer_error_count++;
    
    char *prefix = "\033[1;31merror: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    return;
}

/**
 * @brief Used to print a case of Parser Errors
 * 
 * @param fmt Error Message
 * @param ... 
 */
void parser_error(const char *fmt, ...) {
    parser_error_count++;

    char *prefix = "\033[1;31merror: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    return;
}

/**
 * @brief Get the lexer error count object
 * 
 * @return int Number of Errors
 */
int get_lexer_error_count(void) {
    return lexer_error_count;
}

/**
 * @brief Get the parser error count object
 * 
 * @return int Number of Parser Errors
 */
int get_parser_error_count(void) {
    return parser_error_count;
}

/**
 * @brief Reset the error count for Lexer and Parser
 * 
 */
void reset_error_count(void) {
    lexer_error_count = 0;
    parser_error_count = 0;
    return;
}

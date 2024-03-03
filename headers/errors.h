#pragma once

void error(const char *fmt, ...);
void lexer_error(const char *fmt, ...);
void parser_error(const char *fmt, ...);
int get_lexer_error_count(void);
int get_parser_error_count(void);
void reset_error_count(void);

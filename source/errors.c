#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

static int error_count = 0;
static int warning_count = 0;

void warn(const char *fmt, ...) {
    warning_count++;
    char *prefix = "\033[1;33mwarning: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    return;
}

void error(const char *fmt, ...) {
    error_count++;
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

int get_error_count(void) {
    return error_count;
}

int get_warning_count(void) {
    return warning_count;
}

void reset_error_count(void) {
    error_count = 0;
    return;
}

// maybe a reset warning count between compiler stages?

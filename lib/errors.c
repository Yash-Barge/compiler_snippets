#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void warn(const char *fmt, ...) {
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
    char *prefix = "\033[1;31merror: \033[0m";
    char fmt_mod[strlen(fmt) + strlen(prefix) + 1];
    strcpy(fmt_mod, prefix);
    strcat(fmt_mod, fmt);

    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt_mod, arg);
    va_end(arg);

    exit(1);

    return;
}

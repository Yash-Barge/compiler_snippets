#ifndef H_ERRORS
#define H_ERRORS

void warn(const char *fmt, ...);
void error(const char *fmt, ...);
int get_error_count(void);
int get_warning_count(void);

#endif

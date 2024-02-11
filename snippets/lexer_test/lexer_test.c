#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regex/regex.h"

#define TEMP(str) printf("%s, index=%d\n", str, tk_from_string(str))
#define IS_WHITESPACE(character) (character == ' ' || (character >= 9 && character <= 13))
#define IS_ALPHA(character) ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
#define IS_NUMERIC(character) (character >= '0' && character <= '9')
#define IS_ALPHANUMERIC(character) (IS_ALPHA(character) || IS_NUMERIC(character))
#define ASSIGN_STRING(var, str) \
    do { \
        var = malloc(sizeof(char) * (strlen(str) + 1)); \
        strcpy(var, str); \
    } while (0)
#define SWAP_POINTER(x, y) \
    do { \
        void *temp = x; \
        x = y; \
        y = temp; \
    } while (0)
#define SWAP_INT(x, y) \
    do { \
        x ^= y; \
        y ^= x; \
        x ^= y; \
    } while (0)
#define FILL_BUFFER(buf, buf_len, buf_cap, filestream) \
    do { \
        buf_len = fread(buf, sizeof(buf[0]), buf_cap, filestream); \
        buf[buf_len] = '\0'; \
    } while (0)

// Lexicographically sorted, specifically to use binary search
const char *const token_list[] = {
    "!=",
    "&&&",
    "(",
    ")",
    "*",
    "+",
    ",",
    "-",
    ".",
    "/",
    ":",
    ";",
    "<",
    "<---",
    "<=",
    "==",
    ">",
    ">=",
    "@@@",
    "[",
    "]",
    "_main",
    "as",
    "call",
    "definetype",
    "else",
    "end",
    "endif",
    "endrecord",
    "endunion",
    "endwhile",
    "global",
    "if",
    "input",
    "int",
    "list",
    "output",
    "parameter",
    "parameters",
    "read",
    "real",
    "record",
    "return",
    "then",
    "type",
    "union",
    "while",
    "with",
    "write",
    "~"
};

// Used for tokens identified by regex
enum ENUM_ID {
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_FUNID,
    TK_RUID,
    TK_RNUM
};

char *regular_expressions[] = {
    "[alp][alp]*",
    "[b-d][2-7][b-d]*[2-7]*",
    "[0-9][0-9]*",
    "[_-_][alp][alp]*[0-9]*",
    "[#-#][alp][alp]",
    "[0-9][0-9]*[.-.][0-9][0-9]",
    "[0-9][0-9]*[.-.][0-9][0-9][E-E][+-+][0-9][0-9]",
    "[0-9][0-9]*[.-.][0-9][0-9][E-E][---][0-9][0-9]",
    "[0-9][0-9]*[.-.][0-9][0-9][E-E][0-9][0-9]",
};

/**
 * @param expr expression that is to be checked
 * @return int returns integer index of ENUM_ID of which it is a part, else returns -1
 */
int typeof_identifier(char *expr) {
    for (int i = 0; i < 5; i++) {
        if (check(regular_expressions[i], expr)) return i;
    }
    if (check(regular_expressions[5], expr) || check(regular_expressions[6], expr) || check(regular_expressions[7], expr) || check(regular_expressions[8], expr)) return 5;
    return -1;
}

// Used to index into token_list
enum ENUM_TOK {
    TOKEN_INVALID = -1,
    TK_NE,
    TK_AND,
    TK_OP,
    TK_CL,
    TK_MUL,
    TK_PLUS,
    TK_COMMA,
    TK_MINUS,
    TK_DOT,
    TK_DIV,
    TK_COLON,
    TK_SEMI,
    TK_LT,
    TK_ASSIGNOP,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_OR,
    TK_SQL,
    TK_SQR,
    TK_MAIN,
    TK_AS,
    TK_CALL,
    TK_DEFINETYPE,
    TK_ELSE,
    TK_END,
    TK_ENDIF,
    TK_ENDRECORD,
    TK_ENDUNION,
    TK_ENDWHILE,
    TK_GLOBAL,
    TK_IF,
    TK_INPUT,
    TK_INT,
    TK_LIST,
    TK_OUTPUT,
    TK_PARAMETER,
    TK_PARAMETERS,
    TK_READ,
    TK_REAL,
    TK_RECORD,
    TK_RETURN,
    TK_THEN,
    TK_TYPE,
    TK_UNION,
    TK_WHILE,
    TK_WITH,
    TK_WRITE,
    TK_NOT,
    TOKEN_COUNT // Just to show count of tokens
};

// Binary search, only works as token_list is lexicographically sorted
enum ENUM_TOK tk_from_string(const char *const str) {
    int bottom = 0;
    int top = TOKEN_COUNT - 1;
    int valid_token = 0;

    while (top >= bottom) {
        int comparison = strcmp(str, token_list[(top + bottom) / 2]);

        if (!comparison) {
            valid_token = 1;
            break;
        } else if (comparison > 0)
            bottom = (top + bottom) / 2 + 1;
        else
            top = (top + bottom) / 2 - 1;
    }

    if (!valid_token)
        return TOKEN_INVALID;

    return (top + bottom) / 2;
}

// int main(void) {
//     TEMP("~");
//     TEMP("<---");
//     TEMP("end");
//     TEMP("then");
//     TEMP("asdf");
//     TEMP("endwhile");
//     TEMP("*903cK");
//     TEMP("write");
//     TEMP("!=");

//     return 0;
// }

void consume_whitespace(const char *const buffer, int max_buf_length, int *restrict index, int *restrict line_count) {
    while (*index < max_buf_length && IS_WHITESPACE(buffer[(*index)])) {
        if (buffer[(*index)] == '\n')
            (*line_count)++;
        (*index)++;
    }

    return;
}

char **get_token_list(char *source_path) {
    FILE *source = fopen(source_path, "r");

    const int BUFFER_CAPACITY = 1;

    char *buffer1 = malloc(sizeof(char) * (BUFFER_CAPACITY + 1));
    char *buffer2 = malloc(sizeof(char) * (BUFFER_CAPACITY + 1));
    buffer1[BUFFER_CAPACITY] = '\0';
    buffer2[BUFFER_CAPACITY] = '\0';

    int buffer1_length;
    FILL_BUFFER(buffer1, buffer1_length, BUFFER_CAPACITY, source);
    int buffer2_length, line_number = 1;

    char *tok_incomplete = NULL;
    int index = 0;

    while (1) {
        FILL_BUFFER(buffer2, buffer2_length, BUFFER_CAPACITY, source);

        while (index < buffer1_length) {
            if (buffer1[index] == '%') {
            continue_comment_removal:
                while (buffer1[index] != '\n' && index < buffer1_length)
                    index++;

                if (buffer1[index] == '\n') {
                    line_number++;
                    index++;
                    continue;
                } else { // at end of buffer
                    SWAP_POINTER(buffer1, buffer2);
                    SWAP_INT(buffer1_length, buffer2_length);

                    if (!buffer1_length) // EOF
                        break;

                    FILL_BUFFER(buffer2, buffer2_length, BUFFER_CAPACITY, source);
                    index = 0;
                    goto continue_comment_removal;
                }
            } else if (IS_WHITESPACE(buffer1[index])) {
                consume_whitespace(buffer1, buffer1_length, &index, &line_number);
                continue;
            } else {
            consume_token:;
                int f_index = index;
                char *tok = NULL;

                while (!IS_WHITESPACE(buffer1[f_index]) && f_index < buffer1_length) // temporary, will be replaced by the dfa implementation later
                    f_index++;

                char *temp = malloc(sizeof(char) * (f_index - index + 1));

                for (int i = index; i < f_index; i++)
                    temp[i - index] = buffer1[i];

                temp[f_index - index + 1] = '\0';

                index = f_index;

                if (tok_incomplete != NULL) {
                    tok_incomplete = realloc(tok_incomplete, sizeof(char) * (strlen(tok_incomplete) + strlen(temp) + 1));
                    strcat(tok_incomplete, temp);
                    free(temp);

                    if (index == buffer1_length) {
                        SWAP_POINTER(buffer1, buffer2);
                        SWAP_INT(buffer1_length, buffer2_length);

                        if (!buffer1_length) // EOF
                            goto tok_completed;

                        FILL_BUFFER(buffer2, buffer2_length, BUFFER_CAPACITY, source);
                        index = 0;
                        goto consume_token;
                    } else {
                    tok_completed:
                        tok = tok_incomplete;
                        tok_incomplete = NULL;
                    }
                } else if (index == buffer1_length) {
                    tok_incomplete = temp;
                    SWAP_POINTER(buffer1, buffer2);
                    SWAP_INT(buffer1_length, buffer2_length);

                    if (!buffer1_length) // EOF
                        goto tok_completed;

                    FILL_BUFFER(buffer2, buffer2_length, BUFFER_CAPACITY, source);
                    index = 0;
                    goto consume_token;
                } else {
                    tok = temp;
                }

                printf("%s\n", tok);

                free(tok);
                tok = NULL;

                continue;
            }
        }

        if (!buffer1_length) // EOF
            break;

        SWAP_POINTER(buffer1, buffer2);
        SWAP_INT(buffer1_length, buffer2_length);

        index = 0;
    }

    free(buffer1);
    free(buffer2);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments! Execute program followed by source file!\n");
        return 1;
    }

    // char **source_tok_list = get_token_list(argv[1]);
    get_token_list(argv[1]);

    // for (int i = 0; i < 1024; i++) {
    //     if (source_tok_list[i] == NULL)
    //         break;

    //     printf("%s\n", source_tok_list[i]);
    // }

    return 0;
}

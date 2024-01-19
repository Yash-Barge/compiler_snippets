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
#define NEXT_BUFFER(buf1, buf2, buf_cap, src, ind, f_ind) \
    do { \
        SWAP_POINTER(buf1, buf2); \
        char *temp = fgets(buf2, buf_cap, src); \
        if (temp == NULL) { \
            free(buf2); \
            buf2 = NULL; \
        } \
        ind = 0; \
        f_ind = 0; \
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
        if (buffer[(*index)] == 10)
            (*line_count)++;
        (*index)++;
    }

    return;
}

// TODO: Rewrite to use twin buffer
char *consume_next_token(FILE *source, char *const buffer, int max_buf_length, int *restrict index, int *restrict line_count) {
    char *tok = NULL;
    char *tok_incomplete = NULL;

consume_next_token_redo:

    if (IS_WHITESPACE(buffer[(*index)])) {
        // (After goto) Token is complete, line just happened to be max length of buffer
        if (tok_incomplete != NULL)
            return tok_incomplete;

        consume_whitespace(buffer, max_buf_length, index, line_count);
    }

    // Comment removal
    if (buffer[(*index)] == '%') {
        while (buffer[(*index)] != '\n') {
            if (buffer[(*index)] == '\0') {
                if (fgets(buffer, max_buf_length, source) == NULL)
                    break;
                (*index) = 0;
            }
            (*index)++;
        }

        consume_whitespace(buffer, max_buf_length, index, line_count);
    }

    int forward_index = *index;

    // For function, union and record names
    if (buffer[forward_index] != '\0' && !IS_WHITESPACE(buffer[forward_index]) && (buffer[forward_index] == '_' || buffer[forward_index] == '#')) {
        forward_index++;
        while (buffer[forward_index] != '\0' && !IS_WHITESPACE(buffer[forward_index]) && IS_ALPHA(buffer[forward_index]))
            forward_index++;
    // Variable names
    } else if (buffer[forward_index] != '\0' && !IS_WHITESPACE(buffer[forward_index]) && IS_ALPHA(buffer[forward_index])) {
        while (buffer[forward_index] != '\0' && !IS_WHITESPACE(buffer[forward_index]) && IS_ALPHANUMERIC(buffer[forward_index]))
            forward_index++;
    // Integers
    } else if (buffer[forward_index] != '\0' && !IS_WHITESPACE(buffer[forward_index]) && IS_NUMERIC(buffer[forward_index])) {
        while (buffer[forward_index] != '\0' && !IS_WHITESPACE(buffer[forward_index]) && IS_NUMERIC(buffer[forward_index]))
            forward_index++;
    // Floats???
    // Non-alphanumeric characters (excluding '_' and "#" at the start of tokens)
    } else {
        if (buffer[forward_index] == '\0')
            return NULL;

        switch (buffer[forward_index]) {
        case '!':
            if (buffer[forward_index + 1] == '\0') { // in next buffer
                // EOF
                if (fgets(buffer, max_buf_length, source) == NULL)
                    goto consume_next_token_special_character_default;

                (*index) = 0;

                if (buffer[0] == '=') {
                    ASSIGN_STRING(tok, token_list[TK_NE]);
                    (*index) += 1;
                } else {
                    ASSIGN_STRING(tok, "!");
                }
            } else if (buffer[forward_index + 1] == '=') {
                ASSIGN_STRING(tok, token_list[TK_NE]);
                (*index) += 2;
            } else
                goto consume_next_token_special_character_default;
            break;
        case '=':
            if (buffer[forward_index + 1] == '\0') { // in next buffer
                // EOF
                if (fgets(buffer, max_buf_length, source) == NULL)
                    goto consume_next_token_special_character_default;

                (*index) = 0;

                if (buffer[0] == '=') {
                    ASSIGN_STRING(tok, token_list[TK_EQ]);
                    (*index) += 1;
                } else {
                    ASSIGN_STRING(tok, "=");
                }
            } else if (buffer[forward_index + 1] == '=') {
                ASSIGN_STRING(tok, token_list[TK_EQ]);
                (*index) += 2;
            } else
                goto consume_next_token_special_character_default;
            break;
        case '>':
            if (buffer[forward_index + 1] == '\0') {
                // EOF
                if (fgets(buffer, max_buf_length, source) == NULL)
                    goto consume_next_token_special_character_default;

                (*index) = 0;

                if (buffer[0] == '=') {
                    ASSIGN_STRING(tok, token_list[TK_GE]);
                    (*index) += 1;
                } else {
                    ASSIGN_STRING(tok, token_list[TK_GT]);
                }
            } else if (buffer[forward_index + 1] == '=') {
                ASSIGN_STRING(tok, token_list[TK_GE]);
                (*index) += 2;
            } else
                goto consume_next_token_special_character_default;
            break;
        default:
        consume_next_token_special_character_default:
            tok = malloc(sizeof(char) * 2);
            tok[0] = buffer[(*index)++];
            tok[1] = '\0';
            break;
        }

        return tok;
    }

    tok = malloc(sizeof(char) * (forward_index - (*index) + 1));

    for (int i = (*index); i < forward_index; i++)
        tok[i - (*index)] = buffer[i];

    tok[forward_index - (*index)] = '\0';

    (*index) = forward_index;

    // (After goto) concatenate and complete token
    if (tok_incomplete != NULL) {
        tok_incomplete = realloc(tok_incomplete, sizeof(char) * (strlen(tok_incomplete) + strlen(tok) + 1));
        strcat(tok_incomplete, tok);
        tok = tok_incomplete;
        tok_incomplete = NULL;
    }

    // Buffer does not contain the entire line
    if (forward_index == max_buf_length - 1) {
        tok_incomplete = tok;
        tok = NULL;

        if (fgets(buffer, max_buf_length, source)) {
            (*index) = 0;
            goto consume_next_token_redo;
        } else { // EOF, token is actually complete
            tok = tok_incomplete;
            tok_incomplete = NULL;
        }
    }

    return tok;
}

// char *consume_next_token(FILE *new_source) {
//     const int BUFFER_CAPACITY = 128;

//     static char *buffer_1 = NULL;
//     static char *buffer_2 = NULL;

//     static FILE *source = NULL;
//     static int line_count = 1;
//     static int index = 0;

//     char *tok = NULL;

//     if (new_source != NULL) {
//         source = new_source;
//         line_count = 1;
//         index = 0;

//         if (buffer_1 == NULL)
//             buffer_1 = malloc(sizeof(char) * BUFFER_CAPACITY);
//         if (buffer_2 == NULL)
//             buffer_2 = malloc(sizeof(char) * BUFFER_CAPACITY);

//         char *temp_1 = fgets(buffer_1, BUFFER_CAPACITY, source);
//         char *temp_2 = fgets(buffer_2, BUFFER_CAPACITY, source);

//         if (temp_1 == NULL) {
//             free(buffer_1);
//             buffer_1 = NULL;
//             free(buffer_2);
//             buffer_2 = NULL;
//         } else if (temp_2 == NULL) {
//             free(buffer_2);
//             buffer_2 = NULL;
//         }

//         return NULL;
//     } else if (source == NULL) {
//         return NULL;
//     }

// consume_next_token_redo:

//     if (buffer_1 == NULL)
//         return NULL;

//     if (IS_WHITESPACE(buffer_1[index])) {
//         consume_whitespace(buffer_1, BUFFER_CAPACITY, &index, &line_count);
//     }

//     if (buffer_1[index] == '\0') {
//         NEXT_BUFFER(buffer_1, buffer_2, BUFFER_CAPACITY, source, index, index); // index done twice, as macro accepts 2 forward_index as 2nd index

//         goto consume_next_token_redo;
//     }

//     // Comment removal
//     if (buffer_1[index] == '%') {
//         while (buffer_1[index] != '\n') {
//             if (buffer_1[index] == '\0') {
//                 if (buffer_2 == NULL) {
//                     free(buffer_1);
//                     buffer_1 = NULL;
//                     return NULL;
//                 }

//                 NEXT_BUFFER(buffer_1, buffer_2, BUFFER_CAPACITY, source, index, index); // index done twice, as macro accepts 2 forward_index as 2nd index
//             }
//             index++;
//         }

//         goto consume_next_token_redo;
//     }
    
//     int forward_index = index;

//     // // For function, union and record names
//     // if (buffer_1[forward_index] == '_' || buffer_1[forward_index] == '#')
//     //     forward_index++;

//     // while (buffer_1[forward_index] != '\0' && !IS_WHITESPACE(buffer_1[forward_index]) && IS_ALPHANUMERIC(buffer_1[forward_index]))
//     //     forward_index++;

//     // tok = malloc(sizeof(char) * (forward_index - index + 1));

//     // for (int i = index; i < forward_index; i++)
//     //     tok[i - index] = buffer_1[i];

//     // tok[forward_index - index] = '\0';

//     // index = forward_index;

//     return tok;
// }

char **get_token_list(char *source_path) {
    const int temp_list_size = 1024;
    char **temp_list = malloc(sizeof(char *) * temp_list_size);
    int list_index = 0;

    FILE *source = fopen(source_path, "r");

    if (source == NULL) {
        fprintf(stderr, "Cannot open source file %s! Are you sure the path and filename are correct?\n", source_path);
        exit(1);
    }

    const int buffer_capacity = 12;
    char buffer[buffer_capacity];
    int line_number = 1;

    while (fgets(buffer, buffer_capacity, source)) {
        int i = 0;
        while (i < strlen(buffer)) {
            char *seq = consume_next_token(source, buffer, buffer_capacity, &i, &line_number);
            if (seq != NULL)
                temp_list[list_index++] = seq;
        }
    }

    // char *seq = NULL;

    // while (seq = consume_next_token(source))
    //     temp_list[list_index++] = seq;

    fclose(source);

    return temp_list;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments! Execute program followed by source file!\n");
        return 1;
    }

    char **source_tok_list = get_token_list(argv[1]);

    for (int i = 0; i < 1024; i++) {
        if (source_tok_list[i] == NULL)
            break;

        printf("%s\n", source_tok_list[i]);
    }

    return 0;
}

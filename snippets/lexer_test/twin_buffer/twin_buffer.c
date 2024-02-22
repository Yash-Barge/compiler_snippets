#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "twin_buffer.h"

#define INIT_BUFFER(twin_buf) \
    twin_buf->size = fread(twin_buf->buffer, sizeof(twin_buf->buffer[0]), twin_buf->capacity, twin_buf->source)

/**
 * @brief field "capacity" represents capacity of a singular buffer in the twin buffer, actually capacity of the buffer is capacity * 2
 */
struct twin_buf {
    char *buffer;
    int index;
    int f_index;
    int capacity;
    int size;
    FILE *source;
    char *tok_incomplete;
};

struct twin_buf *tb_init(FILE *source, int capacity) {
    struct twin_buf *tb = malloc(sizeof(struct twin_buf));
    memset(tb, 0, sizeof(*tb));
    *tb = (struct twin_buf) { .buffer = malloc(sizeof(char) * capacity * 2), .capacity = capacity, .source = source };
    INIT_BUFFER(tb);

    return tb;
}

char *tb_consume(struct twin_buf *tb) {
    char *tok = malloc(sizeof(char) * (tb->f_index - tb->index + 1));

    for (int i = tb->index; i < tb->f_index; i++)
        tok[i - tb->index] = tb->buffer[i];
    tok[tb->f_index - tb->index + 1] = '\0';

    if (tb->tok_incomplete) { // Token too long for buffer

        if (tb->f_index != tb->index) { // Checking if end of buffer co-incided with end of token
            tb->tok_incomplete = realloc(tb->tok_incomplete, sizeof(char) * (strlen(tb->tok_incomplete) + strlen(tok) + 1));
            strcat(tb->tok_incomplete, tok);
        }

        free(tok);
        tok = tb->tok_incomplete;
        tb->tok_incomplete = NULL;
    }

    tb->index = tb->f_index;

    if (tb->index >= tb->capacity) {
        memcpy(tb->buffer, tb->buffer + tb->capacity, tb->capacity);
        tb->index -= tb->capacity;
        tb->f_index = tb->index;

        // fread??? how to handle size?
    }

    return tok;
}

char tb_peek(struct twin_buf *tb) {
    return tb->buffer[tb->f_index];
}

void tb_increment(struct twin_buf *tb) {


    return;
}

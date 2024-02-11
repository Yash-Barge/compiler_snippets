#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "buffer.h"
#include "errors.h"

BUFFER * createBuffers(){
    BUFFER *buf = (BUFFER *)malloc(sizeof(BUFFER));

    if (!buf) {
        printf("Error in alloting memory for buffer!");
        return NULL;
    }

    buf->buf1 = (char *)calloc(BUF_SIZE, sizeof(char));
    buf->buf2 = (char *)calloc(BUF_SIZE, sizeof(char));

    if (!buf->buf1 || !buf->buf2) {
        printf("Error in alloting memory for buffer!\n");
        return NULL;
    }

    buf->currentBuffer = 1;
    buf->startBuffer = 1;
    buf->forward = 0;
    buf->init = false;
    buf->start = 0;
    buf->size=0;
    buf->advancedSize=0;
    buf->advanceRead=false;

    return buf;
}

IOHandler* createIOHandler(char* fileName){
    IOHandler* io= (IOHandler *)malloc(sizeof(IOHandler));

    if(!io){
        printf("Error in alloting memory for IOHandler!\n");
        return NULL;
    }

    io->file_ptr = fopen(fileName, "r");

    if (io->file_ptr == NULL) {
        error("Cannot open file %s (%s)\n", fileName, strerror(errno));
        exit(0);
    }

    io->buf = createBuffers();
    io->EOFReached = false;
    io->inputFin = false;
    io->lineNumber = 1;

    return io;
}

int readFile(IOHandler* io) {

    if (io->buf->advanceRead) {
        io->buf->size = io->buf->advancedSize;
        io->buf->advancedSize = 0;
        io->buf->advanceRead = false;
        io->buf->forward = 0;
        io->buf->currentBuffer = io->buf->currentBuffer == 2 ? 1 : 2;

        return 0;
    }

    if (feof(io->file_ptr)) {
        io->EOFReached = true;
        return 1;
    }

    if (io->buf->init && io->buf->currentBuffer == 1) {
        io->buf->size = fread(io->buf->buf2, sizeof(char), BUF_SIZE, io->file_ptr);
    }

    if (io->buf->init && io->buf->currentBuffer == 2) {
        io->buf->size = fread(io->buf->buf1, sizeof(char), BUF_SIZE, io->file_ptr);
    }

    io->buf->currentBuffer = io->buf->currentBuffer == 2 ? 1 : 2;
    io->buf->forward = 0;

    if (!io->buf->init) {
        io->buf->size = fread(io->buf->buf1, sizeof(char), BUF_SIZE, io->file_ptr);
        io->buf->init = true;
        io->buf->currentBuffer = 1;
    }

    if (ferror(io->file_ptr)) {
        printf("File IO Error!\n");
        return -1;
    }

    return 0;    
}

char getChar(IOHandler* io, bool isStart) {

    if (io->buf->forward == io->buf->size) {
        if (!io->EOFReached) {
            int read = readFile(io);
            if (read < 0) {
                printf("IO Error!\n");
            } else if (read == 1) {
                io->inputFin = true;
                return '\0';
            }
        } else {
            io->inputFin = true;
            return '\0';
        }
    }

    if (!io->buf->init) {
        int read = readFile(io);
        if (read < 0) {
            printf("IO Error!\n");
            return '\0';
        }
    }

    char ret;

    if (isStart) {
        io->buf->startBuffer = io->buf->currentBuffer;
        io->buf->start = io->buf->forward;
    }

    if (io->buf->currentBuffer == 1)
        ret = io->buf->buf1[io->buf->forward++];
    else
        ret = io->buf->buf2[io->buf->forward++];

    return ret;
}

int retract(IOHandler* io) {

    // reading at the EOF does not increment the forward pointer, and returns a null character.
    // thus, we mark it as not finished, but do not decrement the forward pointer.
    if (io->inputFin) {
        io->inputFin = false;
        return 0;
    }

    if (io->buf->forward != 0) {
        io->buf->forward--;
    } else {
        io->buf->advanceRead = true;
        io->buf->advancedSize = io->buf->size;
        io->buf->size = BUF_SIZE;
        io->buf->forward = BUF_SIZE;
        io->buf->currentBuffer = io->buf->currentBuffer == 1 ? 2 : 1;
    }

    return 0;
}

int closeHandler(IOHandler* io){
    free(io->buf->buf1);
    free(io->buf->buf2);
    free(io->buf);
    fclose(io->file_ptr);
    free(io);

    return 0;
}

char *getStartBuffer(IOHandler *io){
    if (io->buf->startBuffer==1)
        return io->buf->buf1;
    else
        return io->buf->buf2;
}

char *getCurrentBuffer(IOHandler *io){
    if (io->buf->currentBuffer==1)
        return io->buf->buf1;
    else
        return io->buf->buf2;
}

char *getLexeme(IOHandler *io){
    int lexemeSize = 0;

    if (io->buf->startBuffer == io->buf->currentBuffer)
        lexemeSize = io->buf->forward - io->buf->start;
    else
        lexemeSize = io->buf->forward + BUF_SIZE - io->buf->start;

    char * lexeme = calloc(sizeof(char), lexemeSize + 1);

    if (io->buf->startBuffer == io->buf->currentBuffer) {
        strncpy(lexeme, getCurrentBuffer(io) + io->buf->start, lexemeSize);
    } else {
        strncpy(lexeme, getStartBuffer(io) + io->buf->start, BUF_SIZE - io->buf->start);
        strncpy(lexeme + BUF_SIZE - io->buf->start, getCurrentBuffer(io), io->buf->forward);
    }

    return lexeme;
}

void ignore_read_characters(IOHandler *io) {
    io->buf->startBuffer = io->buf->currentBuffer;
    io->buf->start = io->buf->forward;

    return;
}

void lexical_error(IOHandler *io) {
    char *invalid_lexeme = getLexeme(io);
    error("line %d: Invalid lexeme `%s`\n", io->lineNumber, invalid_lexeme);
    free(invalid_lexeme);

    return;
}

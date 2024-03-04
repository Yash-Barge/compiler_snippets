#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "buffer.h"
#include "errors.h"

/**
 * @brief Create a Buffer instance and initializes all its members
 * 
 * @return pointer to the created buffer instance 
 */
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
    buf->old_size = 0;
    buf->advancedSize=0;
    buf->advanceRead=false;

    return buf;
}

/**
 * @brief creates an IOHandler instance and initializes all its members
 * 
 * @param fileName path of file to be read
 * @return IOHandler* pointer to instance
 */
IOHandler* createIOHandler(char* fileName){
    IOHandler* io= (IOHandler *)malloc(sizeof(IOHandler));

    if(!io){
        printf("Error in alloting memory for IOHandler!\n");
        return NULL;
    }

    io->file_ptr = fopen(fileName, "r");

    if (io->file_ptr == NULL) {
        error("Cannot open file %s (%s)\n", fileName, strerror(errno));
        exit(1);
    }

    io->buf = createBuffers();
    io->EOFReached = false;
    io->inputFin = false;
    io->lineNumber = 1;

    return io;
}

/**
 * @brief reads file stream
 * 
 * @param io IOHandler instance
 * @return int 0 for normal operation, -1 for IO error and 1 if EOF reached
 */
int readFile(IOHandler* io) {

    // Check if the buffer has advanced read
    if (io->buf->advanceRead) {
        io->buf->old_size = io->buf->size;
        io->buf->size = io->buf->advancedSize;
        io->buf->advancedSize = 0;
        io->buf->advanceRead = false;
        io->buf->forward = 0;
        io->buf->currentBuffer = io->buf->currentBuffer == 2 ? 1 : 2;

        return 0;
    }

    // Initialize buffer if not done yet
    if (!io->buf->init) {
        io->buf->old_size = io->buf->size;
        io->buf->size = fread(io->buf->buf1, sizeof(char), BUF_SIZE, io->file_ptr);
        io->buf->init = true;
        io->buf->currentBuffer = 1;
        io->buf->forward = 0;

        return 0;
    }

    // Read into buffer 1 if current buffer is 1
    if (io->buf->init && io->buf->currentBuffer == 1) {
        io->buf->old_size = io->buf->size;
        io->buf->size = fread(io->buf->buf2, sizeof(char), BUF_SIZE, io->file_ptr);
    }

    // Read into buffer 2 if current buffer is 2
    if (io->buf->init && io->buf->currentBuffer == 2) {
        io->buf->old_size = io->buf->size;
        io->buf->size = fread(io->buf->buf1, sizeof(char), BUF_SIZE, io->file_ptr);
    }

    // Check for end of file
    if (io->buf->init && !io->buf->size) {
        io->EOFReached = true;
        return 1;
    }

    // Switch buffer and reset forward position
    io->buf->currentBuffer = io->buf->currentBuffer == 2 ? 1 : 2;
    io->buf->forward = 0;

    // Check for file IO error
    if (ferror(io->file_ptr)) {
        printf("File IO Error!\n");
        return -1;
    }

    return 0;    
}

/**
 * @brief calls readFile in different ways depending on 
 * state of buffer and file
 * 
 * @param io IOHandler instance
 * @param isStart sttart of a new lxeme read
 * @return char 
 */
char getChar(IOHandler* io, bool isStart) {

    // Check if the buffer is initialized
    if (!io->buf->init) {
        int read = readFile(io);
        if (read < 0) {
            printf("IO Error!\n");
            return '\0';
        }
    }

    // Check if the buffer size is zero
    if (!io->buf->size) { // After reaching EOF, io->buf->size would be set to 0 from the fread, cause below pointer checks to fail
        io->inputFin = true;
        return '\0';
    }

     // Check if forward pointer has reached the end of the buffer
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

    char ret;

    // Bring forward and start pointer together if isStart is true
    if (isStart) {
        io->buf->startBuffer = io->buf->currentBuffer;
        io->buf->start = io->buf->forward;
    }

    // Read character from the appropriate buffer
    if (io->buf->currentBuffer == 1)
        ret = io->buf->buf1[io->buf->forward++];
    else
        ret = io->buf->buf2[io->buf->forward++];

    return ret;
}

/**
 * @brief performs retract operation
 * 
 * @param io IOhandler instance
 * @return returns 0
 */
int retract(IOHandler* io) {

    // reading at the EOF does not increment the forward pointer, and returns a null character.
    // thus, we mark it as not finished, but do not decrement the forward pointer.
    // also, as size is set to 0 from fread, we must set it to the old size
    if (io->inputFin) {
        io->inputFin = false;
        io->buf->size = io->buf->old_size;
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

/**
 * @brief frees all parts of IOHandler instance
 * 
 * @param io 
 * @return int 
 */
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

/**
 * @brief Gets the lexeme based on positions of start and forward pointer
 * 
 * @param io 
 * @return char* String containing lexeme
 */
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

// Maybe rename this to `throw_lexer_error` or something
void lexical_error(IOHandler *io) {
    char *invalid_lexeme = getLexeme(io);
    lexer_error("line %4d Error: Invalid lexeme `%s`\n", io->lineNumber, invalid_lexeme);
    free(invalid_lexeme);

    return;
}

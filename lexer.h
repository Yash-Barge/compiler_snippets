/*
    GROUP NUMBER - 27
    Shubham Gupta - 2021A7PS0468P
    Yash Sangram Barge - 2021A7PS0006P
    Venkatavihan Devaki - 2021A7PS0429P
    Priyank Shethia - 2021A7PS0657P
    Aditya Khandelwal - 2021A7PS2422P
*/

#pragma once

#include <stdbool.h>
#include "lexerDef.h"
#include "others.h"

#define BUF_SIZE 256

typedef struct buffer BUFFER;
typedef struct IOHandler IOHandler;

/**
 * @brief Structure representing the twin buffer 
 */
struct buffer{
    char *buf1; //The first buffer                              
    char *buf2; //The second buffer 
    int currentBuffer; //The id of buffer containing forward pointer
    int startBuffer; //The id of buffer containing start pointer
    bool init; // checks if the buffer has been populated initially
    bool advanceRead; //
    int forward; //forward pointer
    int start;// start pointer
    int size; //amount of data in current buffer
    int old_size; //necessary in case of retraction after buffer clear
    int advancedSize; //size of next buffer in case of advanceRead being true
};

/**
 * @brief Handles input from file to program logic
 * 
 */
struct IOHandler{
    FILE* file_ptr; 
    BUFFER *buf;
    int lineNumber;
    bool EOFReached;
    bool inputFin; //when buffer runs out of data
};

//defining functions
BUFFER* createBuffers();
IOHandler* createIOHandler(char* fileName);
int readFile(IOHandler* io);
char getChar(IOHandler* io, bool isStart);
int retract(IOHandler* io);
int closeHandler(IOHandler* io);
char * getStartBuffer(IOHandler *io);
char * getCurrentBuffer(IOHandler *io);
char* getLexeme(IOHandler *io);
void ignore_read_characters(IOHandler *io); // seems to be redundant, check if needed before final submission
void lexical_error(IOHandler *io);

TOKEN* runDFA(IOHandler *io, struct symbol_table *st);
void printToken(TOKEN *token);

/*
    GROUP NUMBER - 27
    Shubham Gupta - 2021A7PS0468P
    Yash Sangram Barge - 2021A7PS0006P
    Venkatavihan Devaki - 2021A7PS0429P
    Priyank Shethia - 2021A7PS0657P
    Aditya Khandelwal - 2021A7PS2422P
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "lexerDef.h"
#include "lexer.h"
#include "others.h"

#define GENERATE_STRINGS(STR) #STR,

/**
 * @brief Array that contains string-versions of the token types, generated
 * maintaining sychronization using FOREACH_TOK macro  
 * 
 */
const char *const TOK_STRING[] = {
    FOREACH_TOK(GENERATE_STRINGS)
};

/**
 * @brief Takes a string and converts it to an enum 
 * 
 * @param tok token
 * @return enum terminals 
 */
enum terminals tok_enumify(const char *restrict const tok) {
    int index = -1;

    for (int i = 0; i < TK_COUNT; i++)
        if (!strcmp(tok, TOK_STRING[i])) {
            index = i;
            break;
        }
    
    return index;
}

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
    lexer_error("Line %4d Error: Invalid lexeme `%s`\n", io->lineNumber, invalid_lexeme);
    free(invalid_lexeme);

    return;
}


/**
 * @brief Prints token line number, token type and lexeme 
 * 
 * @param token token to be printed
 */
void printToken(TOKEN *token) {
    if((int) token->data->token_type == -1) ;
    else printf("%4d\t%15s\t%32s\n", token->lineNumber, (int) token->data->token_type == -1 ? "$": TOK_STRING[token->data->token_type], token->data->stringLexeme);
    return;
}

/**
 * @brief Consumes characters via IOHandler and checks them using 
 * DFA logic until it either runs into a lexical error or tokenizes the accepted input
 * 
 * @param io Pointer to the IOHandler instance 
 * @param st pointer to the symbol table
 * @return TOKEN* returns pointer to token struct that has been created
 */
TOKEN *getNextToken(IOHandler *io, struct symbol_table *st) {
    TOKEN *token = calloc(sizeof(TOKEN), 1);
    int state = 0;
    bool fin = false;
    char ch;

    char *temp = NULL;

    while (!fin) {
        switch (state) {
        case 0:
            ch = getChar(io, state == 0);
            switch (ch) {
            case ',':
                state = 14;
                break;
            case ';':
                state = 15;
                break;
            case ':':
                state = 16;
                break;
            case '.':
                state = 17;
                break;
            case '(':
                state = 18;
                break;
            case ')':
                state = 19;
                break;
            case '+':
                state = 20;
                break;
            case '-':
                state = 21;
                break;
            case '*':
                state = 22;
                break;
            case '/':
                state = 23;
                break;
            case '~':
                state = 26;
                break;
            case '[':
                state = 29;
                break;
            case ']':
                state = 30;
                break;
            case '<':
                state = 51;
                break;
            case '>':
                state = 54;
                break;
            case '@':
                state = 58;
                break;
            case '&':
                state = 56;
                break;
            case '!':
                state = 60;
                break;
            case '=':
                state = 61;
                break;
            case '#':
                state = 49;
                break;
            case '_':
                state = 46;
                break;
            case 'a': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                state = 45;
                break;
            case 'b': case 'c': case 'd':
                state = 42;
                break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 35;
                break;
            case '\n':
                state = 32;
                break;
            case '\t': case ' ': case '\r':
                state = 55;
                break;
            case '%':
                state = 34;
                break;
            case '\0':
                token->data = SymbolTable.insert(st, "$", -1);
                token->lineNumber = io->lineNumber;
                fin = true;

                break;
            default:
                lexical_error(io);
                state = 0;
                break;
            }
            break;
        case 14:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_COMMA);
            fin = true;

            break;
        case 15:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_SEM);
            fin = true;

            break;
        case 16:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_COLON);
            fin = true;

            break;
        case 17:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_DOT);
            fin = true;

            break;
        case 18:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_OP);
            fin = true;

            break;
        case 19:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_CL);
            fin = true;

            break;
        case 20:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_PLUS);
            fin = true;

            break;
        case 21:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_MINUS);
            fin = true;

            break;
        case 22:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_MUL);
            fin = true;

            break;
        case 23:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_DIV);
            fin = true;

            break;
        case 26:
            token->lineNumber = io->lineNumber;
            
            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_NOT);
            fin = true;

            break;
        case 51:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '=':
                state = 8;
                break;
            case '-':
                state = 52;
                break;
            default:
                state = 10;
                break;
            }
            break;
        case 10:
            retract(io);
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_LT);
            fin = true;

            break;
        case 8:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_LE);
            fin = true;

            break;
        case 52:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '-':
                state = 53;
                break;
            default:
                state = 11;
                break;
            }
            break;
        case 11:
            retract(io);
            retract(io);
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_LT);
            fin = true;

            break;
        case 53:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '-':
                state = 9;
                break;
            default:
                state = 69;
                break; ///ERROR 
            }
            break;
        case 9:
            token->lineNumber = io->lineNumber;
    
            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_ASSIGNOP);
            fin = true;

            break;
        case 54:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '=':
                state = 12;
                break;
            default:
                state = 13;
                break;
            }
            break;
        case 12:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_GE);
            fin = true;

            break;
        case 13:
            retract(io);
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_GT);
            fin = true;

            break;
        case 58:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '@':
                state = 59;
                break;
            default:
                state = 69;
                break; ///ERROR 
            }
            break;
        case 59:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '@':
                state = 25;
                break;
            default:
                state = 69;
                break; ///ERROR 
            }
            break;
        case 56:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '&':
                state = 57;
                break;
            default:
                state = 69;
                break; ///ERROR 
            }
            break;
        case 57:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '&':
                state = 24;
                break;
            default:
                state = 69;
                break; ///ERROR   
            }
            break;
        case 24:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_AND);
            fin = true;

            break;
        case 25:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_OR);
            fin = true;

            break;
        case 34:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '\n':
                state = 33;
                break;
            case '\0': // reached EOF, and no newline character after end of comment
                state = 73;
                break;
            default:
                state = 34;
                break;
            }
            break;
        case 33:
            token->lineNumber=io->lineNumber;
            token->data = SymbolTable.insert(st, "", TK_COMMENT);
            io->lineNumber++;
            fin = true;

            break;
        case 60:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '=':
                state = 27;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 61:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '=':
                state = 28;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 27:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_NE);
            fin = true;

            break;
        case 28:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_EQ);
            fin = true;

            break;
        case 35:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 35;
                break;
            case '.':
                state = 36;
                break;
            default:
                state = 1;
                break;
            }
            break;
        case 1:
            retract(io);

            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_NUM);
            fin = true;

            break;
        case 45:
            ch = getChar(io, state == 0);
            switch (ch) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                state = 45;
                break;
            default:
                state = 5;
                break;
            }
            break;
        case 5:
            retract(io);

            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            if (SymbolTable.search_keyword(st, temp))
                token->data = SymbolTable.search_keyword(st, temp);
            else
                token->data = SymbolTable.insert(st, temp, TK_FIELDID);

            fin = true;

            break;
        case 49:
            ch = getChar(io, state == 0);
            switch (ch) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                state = 50;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 50:
            ch = getChar(io, state == 0);
            switch (ch) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                state = 50;
                break;
            default:
                state = 7;
                break;
            }
            break;
        case 46:
            ch = getChar(io, state == 0);
            switch (ch) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                state = 47;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 47:
            ch = getChar(io, state == 0);
            switch (ch) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                state = 47;
                break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 48;
                break;
            default:
                state = 6;
                break;
            }
            break;
        case 48:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 48;
                break;
            default:
                state = 6;
                break;
            }
            break;
        case 7:
            retract(io);

            token->lineNumber = io->lineNumber;
            
            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_RUID);
            fin = true;

            break;
        case 55:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '\t': case ' ': case '\r':
                state = 55;
                break;
            case '\n':
                state = 32;
                break;
            default:
                state = 31;
                break;
            }
            break;
        case 31:
            retract(io);
            state = 0;
            break;
        case 32:
            io->lineNumber++;
            state = 0;
            break;
        case 6:
            retract(io);

            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            if (SymbolTable.search_keyword(st, temp))
                token->data = SymbolTable.search_keyword(st, temp);
            else {
                int length=strlen(temp);
                if(length > 30) {
                    state = 71;
                    break;
                }
                token->data = SymbolTable.insert(st, temp, TK_FUNID);
            }

            fin = true;

            break;
        case 42:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '2': case '3': case '4': case '5': case '6': case '7':
                state = 43;
                break;
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                state = 45;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 43:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '2': case '3': case '4': case '5': case '6': case '7':
                state = 44;
                break;
            case 'b': case 'c': case 'd':
                state = 43;
                break;
            default:
                state = 4;
                break;
            }
            break;
        case 44:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '2': case '3': case '4': case '5': case '6': case '7':
                state = 44;
                break;
            default:
                state = 4;
                break;
            }
            break;
        case 4:
            retract(io);
            //check length                
            token->lineNumber = io->lineNumber;
            temp = getLexeme(io);
            int length=strlen(temp);
            if(length > 20 || length < 2) {
                state = 70;
                break;
            }
            token->data = SymbolTable.insert(st, temp, TK_ID);
            fin = true;

            break;
        case 36:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 37;
                break;
            default:
                state = 62;
                break; ///ERROR
            }
            break;
        case 37:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 38;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 38:
            ch = getChar(io, state == 0);
            switch (ch) {
            case 'E':
                state = 39;
                break;
            default:
                state = 2;
                break;
            }
            break;
        case 2:
            retract(io);
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_RNUM);
            fin = true;

            break;
        case 39:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 40;
                break;
            case '+': case '-':
                state = 41;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 41:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 40;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 40:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 3;
                break;
            default:
                state = 69;
                break; ///ERROR
            }
            break;
        case 3:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_RNUM);
            fin = true;

            break;
        case 69:
            retract(io);
            lexical_error(io);
            state = 0;

            break;
        case 70:
            lexer_error("Line %4d: Variable Identifier is longer than prescribed length of 20 characters.\n", token->lineNumber);
            state = 0;
            temp = NULL;
            
            break;
        case 71:
            lexer_error("Line %4d: Function Identifier is longer than prescribed length of 30 characters.\n", token->lineNumber);
            state = 0;
            temp = NULL;
            
            break;
        case 29:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_SQL);
            fin = true;

            break;
        case 30:
            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_SQR);
            fin = true;

            break;
        case 62:
            retract(io);
            retract(io);

            token->lineNumber = io->lineNumber;

            temp = getLexeme(io);
            token->data = SymbolTable.insert(st, temp, TK_NUM);
            fin = true;

            break;
        case 73:
            retract(io);
            token->lineNumber = io->lineNumber;
            token->data = SymbolTable.insert(st, "", TK_COMMENT);
            fin = true;

            break;
        default:
            state = 69;
            break; ///ERROR (?)
        }
    }

    if (temp) {
        free(temp);
        temp = NULL;
    }

    return token;
}

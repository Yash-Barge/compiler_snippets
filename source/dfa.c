#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dfa.h"

void printToken(TOKEN *token) {
    printf("%4d\t%15s\t%32s\n", token->lineNumber, (int) token->data->token_type == -1 ? "$": TOK_STRING[token->data->token_type], token->data->stringLexeme);
    return;
}

TOKEN *runDFA(IOHandler *io, struct symbol_table *st) {
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
                token->data = SymbolTable.insert(st, "$", -1);
                token->lineNumber = io->lineNumber;
                fin = true;

                break;
            default:
                state = 34;
                break;
            }
            break;
        case 33:
            io->lineNumber++;
            state = 0;
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
            else
                token->data = SymbolTable.insert(st, temp, TK_FUNID);

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
            if(io->buf->forward - io->buf->start)
            temp = getLexeme(io);
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

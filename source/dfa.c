#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dfa.h"
#include "buffer.h"
#include "symboltable.h"

const char *tokenenumnames[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE"
};

TOKEN *createToken() {
    TOKEN *token = calloc(sizeof(TOKEN), 1);

    if (token == NULL) {
        printf("Error in alloting memory for token!\n");
        return NULL;
    }

    token->isError = false;

    return token;
}

void printToken(TOKEN *token) {
    if (token->isError == false)
        printf("%4d\t%15s\t%32s\n", token->lineNumber, tokenenumnames[token->token], token->lexeme);
    else
        printf("Lexical error on line %d in lexeme: %s\n", token->lineNumber, token->lexeme);

    return;
}

// state 32 and 33 are the exact same
TOKEN *runDFA(IOHandler *io, Table symboltable) {
    TOKEN *token = createToken();
    int state = 0;
    bool fin = false;
    char ch;

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
                return NULL;
            default:
                // printf("unknown character (%c)!\n", ch);
                // exit(1);
                // retract(io);
                lexical_error(io);
                state = 0;
                break;
            }
            break;
        case 14:
            token->token = TK_COMMA;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 15:
            token->token = TK_SEM;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 16:
            token->token = TK_COLON;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 17:
            token->token = TK_DOT;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 18:
            token->token = TK_OP;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 19:
            token->token = TK_CL;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 20:
            token->token = TK_PLUS;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 21:
            token->token = TK_MINUS;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 22:
            token->token = TK_MUL;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 23:
            token->token = TK_DIV;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 26:
            token->token = TK_NOT;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
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
            token->token = TK_LT;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 8:
            token->token = TK_LE;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
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
            token->token = TK_LT;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 53:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '-':
                state = 9;
                break;
            default:
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
                state = 69;
                break; ///ERROR 
            }
            break;
        case 9:
            token->token = TK_ASSIGNOP;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
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
            token->token = TK_GE;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 13:
            retract(io);
            token->token = TK_GT;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 58:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '@':
                state = 59;
                break;
            default:
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
                state = 69;
                break; ///ERROR   
            }
            break;
        case 24:
            token->token = TK_AND;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 25:
            token->token = TK_OR;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 34:
            // ignore_read_characters(io);
            ch = getChar(io, state == 0);
            switch (ch) {
            case '\n':
                state = 33;
                break;
            case '\0': // reached EOF, and no newline character after end of comment
                return NULL;
            default:
                state = 34;
                break;
            }
            break;
        case 33:
            // ignore_read_characters(io);
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
                state = 69;
                break; ///ERROR
            }
            break;
        case 27:
            token->token = TK_NE;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 28:
            token->token = TK_EQ;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
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
            //CALCULATE VALUE?
            token->token = TK_NUM;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
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
            token->lexeme = getLexeme(io);
            Node mynode = lookup(token->lexeme, symboltable);
            if (mynode != NULL) {
                token->token = mynode->entry->token_type;
            } else {
                token->token = TK_FIELDID;
            }
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 49:
            ch = getChar(io, state == 0);
            switch (ch) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                state = 50;
                break;
            default:
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
            token->token = TK_RUID;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 55:
            // ignore_read_characters(io);
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
            // ignore_read_characters(io);
            io->lineNumber++;
            state = 0;
            break;
        case 6:
            retract(io);
            // token->token="TK_";
            //IF CONDITION LOOKUP
            token->lexeme = getLexeme(io);
            if (strcmp(token->lexeme, "_main") == 0) {
                token->token = TK_MAIN;
            } else {
                token->token = TK_FUNID;
            }
            token->lineNumber = io->lineNumber;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
            token->token = TK_ID;
            token->lexeme = getLexeme(io);
            //check length                
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 36:
            ch = getChar(io, state == 0);
            switch (ch) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 37;
                break;
            default:
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
            token->token = TK_RNUM;
            //CALCULATE VALUE
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
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
                // printf("ERROR! state = %d\n", state);
                // exit(1);
                // retract(io);
                // lexical_error(io);
                // state = 0;
                state = 69;
                break; ///ERROR
            }
            break;
        case 3:
            token->token = TK_RNUM;
            //CALCULATE VALUE
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 69:
            retract(io);
            lexical_error(io);
            state = 0;
            // token->isError = true;
            // token->lexeme = getLexeme(io);
            // fin = true;
            break;
        case 29:
            token->token = TK_SQL;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 30:
            token->token = TK_SQR;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        case 62:
            retract(io);
            retract(io);
            token->token = TK_NUM;
            token->lexeme = getLexeme(io);
            token->lineNumber = io->lineNumber;
            fin = true;
            break;
        default:
            // printf("ERROR: state not handled!\nstate = %d\n", state);
            // exit(1);
            // retract(io);
            // lexical_error(io);
            // state = 0;
            state = 69;
            break; ///ERROR (?)
        }
    }
    printToken(token);
    return token;
}

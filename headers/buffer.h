#ifndef H_BUFFER
#define H_BUFFER

#include <stdbool.h>

#define BUF_SIZE 32

typedef struct buffer BUFFER;
typedef struct IOHandler IOHandler;

struct buffer{
    char *buf1;
    char *buf2;
    int currentBuffer;
    int startBuffer;
    bool init;
    bool advanceRead;
    int forward;
    int start;
    int size;
    int advancedSize;
};

struct IOHandler{
    FILE* file_ptr;
    BUFFER *buf;
    int lineNumber;
    bool EOFReached;
    bool inputFin;
};

BUFFER* createBuffers();
IOHandler* createIOHandler(char* fileName);
int readFile(IOHandler* io);
char getChar(IOHandler* io, bool isStart);
int retract(IOHandler* io);
int closeHandler(IOHandler* io);
char * getStartBuffer(IOHandler *io);
char * getCurrentBuffer(IOHandler *io);
char* getLexeme(IOHandler *io);
void ignore_read_characters(IOHandler *io);
void lexical_error(IOHandler *io);

#endif

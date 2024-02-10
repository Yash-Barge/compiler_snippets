#define BUF_SIZE 32
#include <stdio.h>
#include <stdbool.h>
typedef struct buffer BUFFER;
typedef struct IOHandler IOHandler;
struct buffer{
    char *buf1;
    char *buf2;
    int currentBuffer;
    int startBuffer;
    bool init;
    bool advanceRead;
    // bool containsData1;
    // bool containsData2;
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
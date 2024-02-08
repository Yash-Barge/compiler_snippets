#define BUF_SIZE 32
#include <stdio.h>

typedef struct buffer BUFFER;
typedef struct IOHandler IOHandler;
struct buffer{
    char *buf1;
    char *buf2;
    int currentBuffer;
    // int startBuffer;
    bool init;
    bool advanceRead;
    // bool containsData1;
    // bool containsData2;
    int forward;
    // int start;
    int size;
    int advancedSize;
};

struct IOHandler{
    FILE* file_ptr;
    BUFFER *buf;
    bool EOFReached;
    bool inputFin;
};


BUFFER* createBuffers();
IOHandler* createIOHandler(char* fileName);
int readFile(IOHandler* io);
char getChar(IOHandler* io);
int retract(IOHandler* io);
int closeHandler(IOHandler* io);
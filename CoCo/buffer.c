#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "buffer.h"
#include <string.h>

BUFFER * createBuffers(){
    BUFFER *buf = (BUFFER *)malloc(sizeof(BUFFER));
    if(!buf){
        printf("Error in alloting memory for buffer!");
        return NULL;
    }
    buf->buf1 = (char *)calloc(BUF_SIZE, sizeof(char));
    buf->buf2 = (char *)calloc(BUF_SIZE, sizeof(char));
    if(!buf->buf1 || !buf->buf2){
        printf("Error in alloting memory for buffer!");
        return NULL;
    }
    buf->currentBuffer = 1;
    buf->startBuffer = 1;
    // buf->containsData1=false;
    // buf->containsData2=false;
    buf->forward=-1;
    buf->init = false;
    buf->start=-1;
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
    io->buf = createBuffers();
    io->EOFReached = false;
    io->inputFin=false;
    io->lineNumber=0;
    return io;
}

int readFile(IOHandler* io){
    if(io->buf->advanceRead==true){
        io->buf->size=io->buf->advancedSize;
        io->buf->advancedSize=0;
        io->buf->advanceRead=false;
        io->buf->currentBuffer = ((io->buf->currentBuffer==2 )?1:2);
        return 0;
    }
    if(io->buf->init==true && io->buf->currentBuffer==1){
        io->buf->size = fread(io->buf->buf2, sizeof(char), BUF_SIZE, io->file_ptr);
    }
    if(io->buf->init==true && io->buf->currentBuffer==2){
        io->buf->size = fread(io->buf->buf1, sizeof(char), BUF_SIZE, io->file_ptr);
    }
    io->buf->currentBuffer = ((io->buf->currentBuffer==2 )?1:2);
    if(io->buf->init==false){
        io->buf->size = fread(io->buf->buf1, sizeof(char), BUF_SIZE, io->file_ptr);
        io->buf->init=true;
        io->buf->currentBuffer=1;
    }
    if(feof(io->file_ptr)){
        io->EOFReached=true;
        printf("EOF Reached!\n");
        return 1;
    }
    if(ferror(io->file_ptr)){
        printf("File IO Error!\n");
        return -1;
    }
    return 0;    
}

char getChar(IOHandler* io, bool isStart){
    if(io->buf->forward==io->buf->size-1){
        if(!io->EOFReached){
            int read = readFile(io);
            if(read<0){
                printf("IO Error!\n");
            }
        }
        else{
            io->inputFin=true;
            return '!';
        }
    }
    if(io->buf->init==false /*|| io->buf->forward==io->buf->size*/){
        int read = readFile(io);
        if(read<0){
            printf("IO Error!\n");
            return '!';
        }
    }
    char ret;
    io->buf->forward = (io->buf->forward+1)%BUF_SIZE;
    if(isStart){
        io->buf->startBuffer=io->buf->currentBuffer;
        io->buf->start=io->buf->forward;
    }
    if(io->buf->currentBuffer==1){
        ret = io->buf->buf1[io->buf->forward];
    }
    else{
        ret = io->buf->buf2[io->buf->forward];
    }
    return ret;
}

int retract(IOHandler* io){
    if(io->buf->forward!=0){
        io->buf->forward--;
    }
    else{
        io->buf->advanceRead = true;
        io->buf->advancedSize=io->buf->size;
        io->buf->size = BUF_SIZE;
        io->buf->forward = BUF_SIZE-1;
        io->buf->currentBuffer = (io->buf->currentBuffer==1?2:1);
    }
    //check later
    if(io->inputFin==true){
        io->inputFin=false;
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

char * getStartBuffer(IOHandler *io){
    if(io->buf->startBuffer==1){
        return io->buf->buf1;
    }
    else{
        return io->buf->buf2;
    }
}

char * getCurrentBuffer(IOHandler *io){
    if(io->buf->currentBuffer==1){
        return io->buf->buf1;
    }
    else{
        return io->buf->buf2;
    }
}

char* getLexeme(IOHandler *io){
    int lexemeSize = io->buf->forward;
    if(io->buf->currentBuffer!=io->buf->startBuffer){
        lexemeSize+=(BUF_SIZE-io->buf->start);
    }
    char * lexeme = (char *)calloc(sizeof(char), lexemeSize+1);
    if(io->buf->startBuffer==io->buf->currentBuffer){
        strncpy(lexeme, getCurrentBuffer(io)+io->buf->start, lexemeSize);
    }
    else{
        strncpy(lexeme, getStartBuffer(io)+io->buf->start, BUF_SIZE-io->buf->start);
        strncpy(lexeme+BUF_SIZE-io->buf->start, getCurrentBuffer(io), io->buf->forward+1);
    }
    lexeme[lexemeSize]='\0';
    return lexeme;
}

// int main(){
//     IOHandler *io=createIOHandler("f1.txt");
//     int i=0;
//     while(!io->inputFin){
//         char c = getChar(io, false);
//         printf("%c", c);
//         i++;
//         if(i==33){
//             retract(io);
//             retract(io);
//             retract(io);
//             retract(io);
//         }
//     }
//     closeHandler(io);
//     return 0;
// }

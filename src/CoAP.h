#ifndef ARDUINODEMO_LIBRARY_H
#define ARDUINODEMO_LIBRARY_H

typedef struct {
    unsigned int Ver : 2;
    unsigned int T : 2;
    unsigned int TKL : 4;
    unsigned int Code : 8;
    unsigned int MessageId : 16;
} Header;



#endif
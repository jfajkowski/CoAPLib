#ifndef ARDUINODEMO_UDPEMULATOR_H
#define ARDUINODEMO_UDPEMULATOR_H


#include <cstdio>
#include <cstring>

class UdpEmulator {
private:
    size_t buffer_size_;
    char* buffer_begin_;
    char* buffer_;

public:
    UdpEmulator(size_t buffer_size);
    ~UdpEmulator();
    void write(const void* source);
    void write(const void* source, size_t num);
    void read(void* destination, size_t num);
};


#endif //ARDUINODEMO_UDPEMULATOR_H

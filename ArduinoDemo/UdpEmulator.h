#ifndef ARDUINODEMO_UDPEMULATOR_H
#define ARDUINODEMO_UDPEMULATOR_H

#include "string.h"

class UdpEmulator {
private:
    unsigned int buffer_size_;
    char* buffer_begin_;
    char* buffer_;
    unsigned int packet_size_;

public:
    UdpEmulator(unsigned int buffer_size);
    ~UdpEmulator();
    void write(const void* source, unsigned int num);
    void read(void* destination, unsigned int num);
    unsigned int parsePacket() const;
};


#endif //ARDUINODEMO_UDPEMULATOR_H

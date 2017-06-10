#include "UdpEmulator.h"

UdpEmulator::UdpEmulator(unsigned int buffer_size) : buffer_size_(buffer_size) {
    buffer_begin_ = new char[buffer_size];
    buffer_ = buffer_begin_;
}

UdpEmulator::~UdpEmulator() {
    delete buffer_begin_;
}

void UdpEmulator::write(const void* source, unsigned int num) {
    memcpy(buffer_begin_, source, num);
    packet_size_ = num;
}

void UdpEmulator::read(void* destination, unsigned int num) {
    memcpy(destination, buffer_, num);
    buffer_ += num;
}

unsigned int UdpEmulator::parsePacket() const {
    return packet_size_;
}

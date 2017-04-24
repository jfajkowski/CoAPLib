#include "UdpEmulator.h"

UdpEmulator::UdpEmulator(size_t buffer_size) : buffer_size_(buffer_size) {
    buffer_begin_ = new char[buffer_size];
    buffer_ = buffer_begin_;
}

UdpEmulator::~UdpEmulator() {
    delete buffer_begin_;
}

void UdpEmulator::write(const void* source) {
    write(source, sizeof(source));
}

void UdpEmulator::write(const void* source, size_t num) {
    memcpy(buffer_begin_, source, num);
}

void UdpEmulator::read(void* destination, size_t num) {
    memcpy(destination, buffer_, num);
    buffer_ += num;
}

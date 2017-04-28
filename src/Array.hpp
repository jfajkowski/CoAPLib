#ifndef ARDUINODEMO_ARRAY_H
#define ARDUINODEMO_ARRAY_H


template <typename T>
class Array {
private:
    unsigned int size_;
    T* array_begin_;
public:
    Array();
    Array(unsigned int size_);
    ~Array();

    unsigned int size() const;
    T *begin() const;
};

template <typename T>
Array<T>::Array() : Array(1) {}

template <typename T>
Array<T>::Array(unsigned int size) : size_(size) {
    array_begin_ = new T[size];
}

template <typename T>
Array<T>::~Array() {
    delete array_begin_;
}

template <typename T>
unsigned int Array<T>::size() const {
    return size_;
}

template <typename T>
T *Array<T>::begin() const {
    return array_begin_;
}

#endif //ARDUINODEMO_ARRAY_H

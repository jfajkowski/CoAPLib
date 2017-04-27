#ifndef ARDUINODEMO_ARRAY_H
#define ARDUINODEMO_ARRAY_H


template <typename T>
class Array {
public:
    unsigned int size();
private:
    unsigned int size_;
    T* array_begin_;
};

template <typename T>
unsigned int Array<T>::size() {
    return size_;
}

#endif //ARDUINODEMO_ARRAY_H

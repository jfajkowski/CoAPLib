#ifndef ARDUINODEMO_ARRAY_H
#define ARDUINODEMO_ARRAY_H


template <typename T>
class Array {
public:
    unsigned int size();
private:
    unsigned int size_;
    T* array_begin_;
public:
    T *getArray_begin_() const;
};

template <typename T>
unsigned int Array<T>::size() {
    return size_;
}

template <typename T>
T *Array<T>::getArray_begin_() const {
    return array_begin_;
}

#endif //ARDUINODEMO_ARRAY_H

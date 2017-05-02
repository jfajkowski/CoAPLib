#ifndef ARDUINODEMO_ARRAY_H
#define ARDUINODEMO_ARRAY_H

template <typename T>
class Array;
typedef Array<unsigned char> ByteArray;

template <typename T>
class Array {
private:
    unsigned int size_;
    unsigned int capacity_;
    T* array_begin_;
public:
    Array();
    Array(unsigned int capacity);
    ~Array();

    void pushBack(const T &value);
    void reserve(unsigned int new_capacity);

    const T &operator[] (int index) const;
    Array &operator+=(const Array &array);

    unsigned int size() const;

    unsigned int capacity() const;

    T *begin() const;
};

template <typename T>
Array<T>::Array() : Array(1) {}

template <typename T>
Array<T>::Array(unsigned int capacity) : capacity_(capacity), size_(0) {
    array_begin_ = new T[capacity];
}

template <typename T>
Array<T>::~Array() {
    delete[] array_begin_;
}

template <typename T>
void Array<T>::pushBack(const T &value) {
    if (size_ == capacity_) {
        reserve(size_ + 1);
    }
    array_begin_[size_] = value;
    ++size_;
}

template <typename T>
void Array<T>::reserve(unsigned int new_capacity) {
    unsigned int capacity = new_capacity > capacity_ ?  capacity_ : new_capacity;

    T* new_array_begin = new T[new_capacity];

    for (int i = 0; i < capacity; ++i) {
        new_array_begin[i] = array_begin_[i];
    }

    delete[] array_begin_;
    array_begin_ = new_array_begin;
    capacity_ = new_capacity;
}

template <typename T>
const T &Array<T>::operator[](int index) const {
    return array_begin_[index];
}

template <typename T>
Array<T> &Array<T>::operator+=(const Array<T> &array) {
    reserve(size_ + array.size_);

    for (int i = 0; i < array.size_; ++i) {
        pushBack(array[i]);
    }

    return *this;
}

template <typename T>
unsigned int Array<T>::size() const {
    return size_;
}

template <typename T>
unsigned int Array<T>::capacity() const {
    return capacity_;
}

template <typename T>
T *Array<T>::begin() const {
    return array_begin_;
}

#endif //ARDUINODEMO_ARRAY_H

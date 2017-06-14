#ifndef ARRAY_H
#define ARRAY_H

#include "../Environment.h"

template <typename T>
class Array;
typedef Array<unsigned char> ByteArray;

/**This class manages size and memory taken by char array used in CoApMessage**/
template <typename T>
class Array {
private:
    unsigned int size_;
    unsigned int capacity_;
    T* array_begin_;
public:
    Array();
    Array(unsigned int capacity);
    Array(const Array & array);

    ~Array();

    void serialize(unsigned char *cursor) const;
    void deserialize(unsigned char *cursor, unsigned int num);

    void pushFront(const T &value);
    void pushBack(const T &value);
    const T popBack();
    void insert(const T &value, unsigned int index);
    const T pop(unsigned int index);
    void erase(unsigned int index);
    void reserve(unsigned int new_capacity);

    Array &operator=(const Array & array);
    const T &operator[] (int index) const;
    Array &operator+=(const Array &array);

    unsigned int size() const;
    unsigned int capacity() const;

    T *begin() const;
    T *end() const;
};

template <typename T>
Array<T>::Array() : Array(0) {}

/**Creates array with reserved memory for given number of elements**/
template <typename T>
Array<T>::Array(unsigned int capacity) : capacity_(capacity), size_(0), array_begin_(nullptr) {
    if (capacity > 0)
        reserve(capacity);
}

/**Creates copy of another Array**/
template <typename T>
Array<T>::Array(const Array &array) {
    T* new_array_begin = new T[array.capacity_];

    if(array.array_begin_!= nullptr) {
        for(int i = 0; i < array.capacity_; i++) {
            new_array_begin[i] = array.array_begin_[i];
        }
    }
    array_begin_ = new_array_begin;
    capacity_ = array.capacity_;
    size_=array.size_;
}

template <typename T>
Array<T>::~Array() {
    delete[] array_begin_;
}

/** Adds given element at front of the array **/
template <typename T>
void Array<T>::pushFront(const T &value) {
    insert(value, 0);
}

/** Adds given element at the end of the array.
 *  If capacity is too small elements will be reallocated to new, bigger array **/
template <typename T>
void Array<T>::pushBack(const T &value) {
    if (size_ == capacity_) {
        reserve(size_ + 1);
    }
    array_begin_[size_] = value;
    ++size_;
}

/** Returns last element and deletes it from array **/
template<typename T>
const T Array<T>::popBack() {
    T element = array_begin_[size_ - 1];
    --size_;
    if(size_ > 0 && size_ < capacity_ - 1)
        reserve(size_);
    return element;
}

/** Returns element at given index and deletes it from array **/
template<typename T>
const T Array<T>::pop(unsigned int index) {
    T element = array_begin_[index];
    T* iterator1 = array_begin_ + index;
    T* iterator2 = array_begin_ + index + 1;
    T* end = Array<T>::end();

    while(iterator2 != end) {
        *iterator1 = *iterator2;
        iterator1++;
        iterator2++;
    }
    --size_;
    if(size_ > 0 && size_ < capacity_ - 1)
        reserve(size_);
    return element;
}

/** Deletes element at given index **/
template<typename T>
void Array<T>::erase(unsigned int index) {
    T* iterator1 = array_begin_ + index;
    T* iterator2 = array_begin_ + index + 1;
    T* end = Array<T>::end();

    while(iterator2 != end) {
        *iterator1 = *iterator2;
        iterator1++;
        iterator2++;
    }
    --size_;
    if(size_ > 0 && size_ < capacity_ - 1)
        reserve(size_);
}

/** Inserts element after given index **/
template <typename T>
void Array<T>::insert(const T &value, unsigned int index) {
    unsigned int old_capacity = capacity_;
    if (index + 1 > size_) {
        if (index + 1 > capacity_)
            capacity_ = index + 1;
        size_ = index + 1;
    }
    else if (size_ == capacity_) {
        ++capacity_;
        ++size_;
    }

    T* new_array_begin = new T[capacity_];

    if (array_begin_ != nullptr) {
        for (int i = 0; i < index; ++i) {
            new_array_begin[i] = array_begin_[i];
        }

        for (int i = index; i < old_capacity; ++i) {
            new_array_begin[i + 1] = array_begin_[i];
        }
        delete[] array_begin_;
    }

    array_begin_ = new_array_begin;
    array_begin_[index] = value;
}

/** Moves all elements to new array with given capacity **/
template <typename T>
void Array<T>::reserve(unsigned int new_capacity) {
    unsigned int capacity = new_capacity > capacity_ ?  capacity_ : new_capacity;

    T* new_array_begin = new T[new_capacity];

    if (array_begin_ != nullptr) {
        for (int i = 0; i < capacity; ++i) {
            new_array_begin[i] = array_begin_[i];
        }
        delete[] array_begin_;
    }

    array_begin_ = new_array_begin;
    capacity_ = new_capacity;
}

/** Returns element at given index**/
template <typename T>
const T &Array<T>::operator[](int index) const {
    return array_begin_[index];
}

/** Pushes element at the back and expands array if neccessary**/
template <typename T>
Array<T> &Array<T>::operator+=(const Array<T> &array) {
    reserve(size_ + array.size_);

    for (int i = 0; i < array.size_; ++i) {
        pushBack(array[i]);
    }

    return *this;
}

/** Returns number of valid elements in the array **/
template <typename T>
unsigned int Array<T>::size() const {
    return size_;
}

/** Returns the capacity of an array (max number of elements that will fit into array without resizing it) **/
template <typename T>
unsigned int Array<T>::capacity() const {
    return capacity_;
}

/** Returns pointer to the first element of the array **/
template <typename T>
T *Array<T>::begin() const {
    return array_begin_;
}

/** Returns pointer to the element after the last valid element of an array **/
template <typename T>
T *Array<T>::end() const {
    return &array_begin_[size_];
}
/**Copies into array content of another array **/
template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &array) {
    if(&array != this) {
        size_ = array.size_;
        reserve(array.capacity_);

        for (int i = 0; i < size_; ++i) {
            array_begin_[i] = array.array_begin_[i];
        }
    }
    return *this;
}
/**Copies contetnt of Array into simple char array **/
template <typename T>
void Array<T>::serialize(unsigned char *cursor) const {
    memcpy(cursor, array_begin_, size_);
}
/**Copies content of char array into our Array **/
template <typename T>
void Array<T>::deserialize(unsigned char *cursor, unsigned int num) {
    reserve(num);
    size_ = capacity_;
    memcpy(array_begin_, cursor, num);
}

#endif //ARRAY_H

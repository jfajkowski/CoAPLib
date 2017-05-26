#include <ArduinoUnit.h>
#include "Array.hpp"

test(PushBackTest) {
    unsigned int expected_size = 0;
    ByteArray array(10);
    unsigned int actual_size = array.size();
    assertEqual(expected_size, actual_size);

    expected_size = 2;
    array.pushBack(1);
    array.pushBack(2);
    actual_size = array.size();
    assertEqual(expected_size, actual_size);
    assertEqual(array[0], 1);
    assertEqual(array[1], 2);
}

test(CapacityTest) {
    unsigned int expected_capacity = 10;
    ByteArray array(expected_capacity);
    unsigned int actual_capacity = array.capacity();
    assertEqual(expected_capacity, actual_capacity);
}

test(ConstructorFromUnsignedCharBuffer) {
    unsigned int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int num = 10 * sizeof(unsigned int);
    unsigned char buffer[num];

    memcpy(&buffer, &values, num);

    Array<unsigned int> array(&buffer, num);

    for (int i = 0; i < 10; ++i) {
        assertEqual(i, array[i]);
    }
}

test(AssignmentTest) {
    unsigned int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int num = 10 * sizeof(unsigned int);
    unsigned char buffer[num];

    memcpy(&buffer, &values, num);

    Array<unsigned int> array(&buffer, num);
    Array<unsigned int> copy(array);

    for (int i = 0; i < 10; ++i) {
        assertEqual(array[i], copy[i]);
    }
}

test(ConcatenateTest) {
    ByteArray a(10);
    for (int i = 0; i < 10; ++i) {
        a.pushBack((unsigned char) i);
    }


    ByteArray b(10);
    for (int i = 10; i < 20; ++i) {
        b.pushBack((unsigned char) i);
    }

    a += b;

    for (int i = 0; i < 10; ++i) {
        assertEqual(a[i], (unsigned char) i);
    }
    for (int i = 10; i < 20; ++i) {
        assertEqual(a[i], b[i - 10]);
    }
}

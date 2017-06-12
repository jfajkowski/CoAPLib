#include "Test.hpp"

beginTest

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

test(InsertIntoEmptyTest) {
    unsigned int expected_size = 6;
    ByteArray array;
    array.insert(1,5);
    unsigned int actual_size = array.size();
    assertEqual(expected_size, actual_size);
    assertEqual(array[5], 1);
}

test(InsertIntoFullTest) {
    unsigned int expected_size = 5;
    ByteArray array;
    array.pushBack(0);
    array.pushBack(0);
    array.insert(1, 0);
    array.insert(1, 2);
    array.insert(1, 4);
    unsigned int actual_size = array.size();
    assertEqual(expected_size, actual_size);
    assertEqual(array[0], 1);
    assertEqual(array[1], 0);
    assertEqual(array[2], 1);
    assertEqual(array[3], 0);
    assertEqual(array[4], 1);
}

test(CapacityTest) {
    unsigned int expected_capacity = 10;
    ByteArray array(expected_capacity);
    unsigned int actual_capacity = array.capacity();
    assertEqual(expected_capacity, actual_capacity);
}

test(DeserializeBufferTest) {
    unsigned int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int num = 10 * sizeof(unsigned int);
    unsigned char buffer[num];
    unsigned char *buffer_begin = buffer;

    memcpy(&buffer, &values, num);

    Array<unsigned int> array;
    array.deserialize(buffer_begin, num);

    for (int i = 0; i < 10; ++i) {
        assertEqual(i, array[i]);
    }
}

test(AssignmentTest) {
    unsigned int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int num = 10 * sizeof(unsigned int);
    unsigned char buffer[num];
    unsigned char *buffer_begin = buffer;

    memcpy(&buffer, &values, num);

    Array<unsigned int> array;
    array.deserialize(buffer_begin, num);
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

    test(IteratorTest) {
        OptionArray array(2);
        assertEqual(array.capacity(), 2);
        array.pushBack(CoAPOption(1, "1st"));
        array.pushBack(CoAPOption(2, "2nd"));
        assertEqual(array.capacity(), 2);
        CoAPOption* begin = array.begin();
        assertEqual(begin->getNumber(), 1);
        begin++;
        assertEqual(begin->getNumber(), 2);
        CoAPOption* end = array.end(); //at this point iterator points at memory that does not belong to array
        begin++;
        assertEqual(begin, end);
    }

endTest
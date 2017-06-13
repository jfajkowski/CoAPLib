#include "Test.hpp"

beginTest

    test(PushBack) {
        unsigned int expected_size = 0;
        ByteArray empty_array(5);
        unsigned int actual_size = empty_array.size();
        assertEqual(expected_size, actual_size);

        ByteArray array(0);
        expected_size = 3;
        array.pushBack(1);
        array.pushBack(2);
        array.pushBack(3);
        actual_size = array.size();
        assertEqual(expected_size, actual_size);
        assertEqual(array[0], 1);
        assertEqual(array[1], 2);
        assertEqual(array[2], 3);
    }

    test(PushFront) {
        ByteArray array;
        unsigned int expected_size = 3;
        array.pushFront(3);
        array.pushFront(2);
        array.pushFront(1);
        unsigned int actual_size = array.size();
        assertEqual(expected_size, actual_size);
        assertEqual(array[0], 1);
        assertEqual(array[1], 2);
        assertEqual(array[2], 3);
    }

    test(InsertIntoEmpty) {
        unsigned int expected_size = 6;
        ByteArray array;
        array.insert(1,5);
        unsigned int actual_size = array.size();
        assertEqual(expected_size, actual_size);
        assertEqual(array[5], 1);
    }

    test(InsertIntoFull) {
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

    test(Capacity) {
        unsigned int expected_capacity = 10;
        ByteArray array(expected_capacity);
        unsigned int actual_capacity = array.capacity();
        assertEqual(expected_capacity, actual_capacity);
    }

    test(DeserializeBuffer) {
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

    test(Assignment) {
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

    test(Concatenate) {
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

    test(Iterator) {
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

    test(PopBack) {
        OptionArray array(2);
        array.pushBack(CoAPOption(1, "1st"));
        array.pushBack(CoAPOption(2, "2nd"));
        assertEqual(array.size(), 2);
        assertEqual(array.capacity(), 2);
        CoAPOption option = array.popBack();
        assertEqual(option.getNumber(), 2);
        assertEqual(array.size(), 1);
        option = array.popBack();
        assertEqual(option.getNumber(), 1);
        assertEqual(array.size(), 0);
        assertEqual(array.capacity(), 2);
    }

    test(Pop) {
        OptionArray array(2);
        array.pushBack(CoAPOption(1, "1st"));
        array.pushBack(CoAPOption(2, "2nd"));
        assertEqual(array.size(), 2);
        assertEqual(array.capacity(), 2);
        CoAPOption option = array.pop(0);
        assertEqual(option.getNumber(), 1);
        assertEqual(array.begin()->getNumber(), 2);
        assertEqual(array.capacity(), 2);
        assertEqual(array.size(), 1);

        array.pushBack(CoAPOption(3, "3rd"));
        array.pushBack(CoAPOption(4, "4th"));
        // now array is {2, 3, 4}
        assertEqual(array.capacity(), 3);
        assertEqual(array.size(), 3);
        option = array.pop(1);
        // should be {2, 4}
        assertEqual(option.getNumber(), 3);
        assertEqual(array.begin()->getNumber(), 2);
        assertEqual(array.capacity(), 3);
        assertEqual(array.size(), 2);
    }

endTest
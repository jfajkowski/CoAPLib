#include <boost/test/unit_test.hpp>
#include "../src/Array.hpp"

BOOST_AUTO_TEST_CASE(PushBackTest) {
    unsigned int expected_size = 0;
    ByteArray array(10);
    unsigned int actual_size = array.size();
    BOOST_CHECK_EQUAL(expected_size, actual_size);

    expected_size = 2;
    array.pushBack(1);
    array.pushBack(2);
    actual_size = array.size();
    BOOST_CHECK_EQUAL(expected_size, actual_size);
    BOOST_CHECK_EQUAL(array[0], 1);
    BOOST_CHECK_EQUAL(array[1], 2);
}

BOOST_AUTO_TEST_CASE(CapacityTest) {
    unsigned int expected_capacity = 10;
    ByteArray array(expected_capacity);
    unsigned int actual_capacity = array.capacity();
    BOOST_CHECK_EQUAL(expected_capacity, actual_capacity);
}

BOOST_AUTO_TEST_CASE(ConstructorFromUnsignedCharBuffer) {
    unsigned int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int num = 10 * sizeof(unsigned int);
    unsigned char buffer[num];

    memcpy(&buffer, &values, num);

    Array<unsigned int> array(&buffer, num);

    for (int i = 0; i < 10; ++i) {
        BOOST_CHECK_EQUAL(i, array[i]);
    }
}

BOOST_AUTO_TEST_CASE(AssignmentTest) {
    unsigned int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int num = 10 * sizeof(unsigned int);
    unsigned char buffer[num];

    memcpy(&buffer, &values, num);

    Array<unsigned int> array(&buffer, num);
    Array<unsigned int> copy = array;

    for (int i = 0; i < 10; ++i) {
        BOOST_CHECK_EQUAL(array[i], copy[i]);
    }
}

BOOST_AUTO_TEST_CASE(ConcatenateTest) {
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
        BOOST_CHECK_EQUAL(a[i], (unsigned char) i);
    }
    for (int i = 10; i < 20; ++i) {
        BOOST_CHECK_EQUAL(a[i], b[i - 10]);
    }
}
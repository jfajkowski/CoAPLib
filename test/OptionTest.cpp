#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#include <boost/test/unit_test.hpp>
#include "../src/Option.h"

BOOST_AUTO_TEST_CASE(SerializationTest) {
    Option option1;
    option1.setDelta(1);
    option1.setValue(ByteArray(1));

    Option option2;
    option2.setDelta(2);
    option2.setValue(ByteArray(2));

    OptionArray optionArray(2);
    optionArray.pushBack(option1);
    optionArray.pushBack(option2);

    ByteArray result = Option::serialize(optionArray);

    BOOST_CHECK_EQUAL(result.size(), 3);

    ByteArray val(3);
    val.pushBack(1);
    val.pushBack(2);
    val.pushBack(3);
    Option option3;
    option3.setDelta(3);
    option3.setValue(val);
    optionArray.pushBack(option3);

    result = Option::serialize(optionArray);
    BOOST_CHECK_EQUAL(result.size(), 7);
    BOOST_CHECK_EQUAL(result[result.size() - 1], PAYLOAD_MARKER);
}

BOOST_AUTO_TEST_CASE(NoOptionsDeserializationTest) {
    ByteArray byteArray;
    unsigned char* buffer;

    OptionArray source;
    byteArray = Option::serialize(source);
    buffer = byteArray.begin();
    OptionArray destination = Option::deserialize(buffer);

    BOOST_CHECK_EQUAL(0, source.size());
    BOOST_CHECK_EQUAL(0, destination.size());
}

BOOST_AUTO_TEST_CASE(NibbleSerializationTest) {
    unsigned char buffer;
    struct Nibble {
        unsigned char lo : 4;
        unsigned char hi : 4;
    };

    Nibble expected = {4, 1};
    memcpy(&buffer, &expected, sizeof(expected));
    Nibble actual;
    memcpy(&actual, &buffer, sizeof(buffer));

    BOOST_CHECK_EQUAL(expected.lo, actual.lo);
    BOOST_CHECK_EQUAL(expected.hi, actual.hi);
}

BOOST_AUTO_TEST_CASE(DeserializationTest) {
    unsigned char values1[] = {0, 1, 2, 3};
    unsigned char values2[] = {3, 2, 1, 0};
    unsigned int num = 4;
    unsigned char* buffer;

    Option expected1;
    expected1.setDelta(1);
    expected1.setValue(ByteArray(values1, num));

    Option expected2;
    expected2.setDelta(2);
    expected2.setValue(ByteArray(values2, num));

    OptionArray expected;
    expected.pushBack(expected1);
    expected.pushBack(expected2);

    ByteArray byteArray = Option::serialize(expected);
    buffer = byteArray.begin();

    OptionArray actual = Option::deserialize(buffer);

    BOOST_CHECK_EQUAL(expected[0].getDelta(), actual[0].getDelta());
    BOOST_CHECK_EQUAL(expected[1].getDelta(), actual[1].getDelta());
    BOOST_CHECK_EQUAL(expected[0].getLength(), actual[0].getLength());
    BOOST_CHECK_EQUAL(expected[1].getLength(), actual[1].getLength());

    for (int i = 0; i < num; ++i) {
        BOOST_CHECK_EQUAL(expected[0].getValue()[i], actual[0].getValue()[i]);
        BOOST_CHECK_EQUAL(expected[1].getValue()[i], actual[1].getValue()[i]);
    }
}

BOOST_AUTO_TEST_CASE(GetterAndSetterTest) {
    Option option;

    for (int i = 0; i <= 65535; i += 10) {
        option.setDelta((unsigned short) i);
        BOOST_CHECK_EQUAL((unsigned short) i, option.getDelta());
    }

    ByteArray ten_length;
    ByteArray one_hundred_length;
    ByteArray one_thousand_length;

    for (int i = 0; i < 1000; ++i) {
        if (i % 100 == 0) ten_length.pushBack(0);
        if (i % 10 == 0) one_hundred_length.pushBack(0);
        one_thousand_length.pushBack(0);
    }

    option.setValue(ten_length);
    BOOST_CHECK_EQUAL(ten_length.size(), option.getLength());
    option.setValue(one_hundred_length);
    BOOST_CHECK_EQUAL(one_hundred_length.size(), option.getLength());
    option.setValue(one_thousand_length);
    BOOST_CHECK_EQUAL(one_thousand_length.size(), option.getLength());
}

#pragma clang diagnostic pop
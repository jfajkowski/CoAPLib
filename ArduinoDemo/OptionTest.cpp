#include <ArduinoUnit.h>

#include "Option.h"

test(SerializationTest) {
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

    assertEqual(result.size(), 3);

    ByteArray val(3);
    val.pushBack(1);
    val.pushBack(2);
    val.pushBack(3);
    Option option3;
    option3.setDelta(3);
    option3.setValue(val);
    optionArray.pushBack(option3);

    result = Option::serialize(optionArray);
    assertEqual(result.size(), 7);
    assertEqual(result[result.size() - 1], PAYLOAD_MARKER);
}

test(NoOptionsDeserializationTest) {
    ByteArray byteArray;
    unsigned char* buffer;

    OptionArray source;
    byteArray = Option::serialize(source);
    buffer = byteArray.begin();
    OptionArray destination = Option::deserialize(buffer);

    assertEqual(0, source.size());
    assertEqual(0, destination.size());
}

test(NibbleSerializationTest) {
    unsigned char buffer;
    struct Nibble {
        unsigned char lo : 4;
        unsigned char hi : 4;
    };

    Nibble expected = {4, 1};
    memcpy(&buffer, &expected, sizeof(expected));
    Nibble actual;
    memcpy(&actual, &buffer, sizeof(buffer));

    assertEqual(expected.lo, actual.lo);
    assertEqual(expected.hi, actual.hi);
}

test(NoExtendedFieldsOptionsDeserializationTest) {
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

    assertEqual(expected[0].getDelta(), actual[0].getDelta());
    assertEqual(expected[1].getDelta(), actual[1].getDelta());
    assertEqual(expected[0].getLength(), actual[0].getLength());
    assertEqual(expected[1].getLength(), actual[1].getLength());

    for (int i = 0; i < num; ++i) {
        assertEqual(expected[0].getValue()[i], actual[0].getValue()[i]);
        assertEqual(expected[1].getValue()[i], actual[1].getValue()[i]);
    }
}

//test(ExtendedFieldsOptionsDeserializationTest) {
//    unsigned char* buffer;
//
//    ByteArray one_hundred_length;
//    ByteArray one_thousand_length;
//
//    for (int i = 0; i < 1000; ++i) {
//        if (i % 10 == 0) one_hundred_length.pushBack(0);
//        one_thousand_length.pushBack(0);
//    }
//
//    Option expected1;
//    expected1.setDelta(100);
//    expected1.setValue(one_hundred_length);
//
//    Option expected2;
//    expected2.setDelta(1000);
//    expected2.setValue(one_thousand_length);
//
//    OptionArray expected;
//    expected.pushBack(expected1);
//    expected.pushBack(expected2);
//
//    ByteArray byteArray = Option::serialize(expected);
//    buffer = byteArray.begin();
//
//    OptionArray actual = Option::deserialize(buffer);
//
//    assertEqual(expected[0].getDelta(), actual[0].getDelta());
//    assertEqual(expected[1].getDelta(), actual[1].getDelta());
//    assertEqual(expected[0].getLength(), actual[0].getLength());
//    assertEqual(expected[1].getLength(), actual[1].getLength());
//
//    assertEqual(expected[0].getValue().size(), actual[0].getValue().size());
//    assertEqual(expected[1].getValue().size(), actual[1].getValue().size());
//}

test(GetterAndSetterTest) {
    Option option;

    for (int i = 0; i <= 65535; i += 10) {
        option.setDelta((unsigned short) i);
        assertEqual((unsigned short) i, option.getDelta());
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
    assertEqual(ten_length.size(), option.getLength());
    option.setValue(one_hundred_length);
    assertEqual(one_hundred_length.size(), option.getLength());
    option.setValue(one_thousand_length);
    assertEqual(one_thousand_length.size(), option.getLength());
}

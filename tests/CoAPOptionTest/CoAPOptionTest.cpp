#include "Test.hpp"

beginTest

test(SingleOptionDeserializationTest) {
    unsigned int buffer_size = 5;
    unsigned char buffer[] = {0xb4, 0x74, 0x65, 0x73, 0x74};
    unsigned char* buffer_begin = buffer;
    unsigned char* buffer_end = buffer + buffer_size;

    OptionArray option_array;
    CoAPOption::deserialize(buffer_begin, buffer_end, option_array);

    assertEqual(option_array.size(), 1);
    assertEqual(option_array[0].getNumber(), 11);
    assertEqual(option_array[0].getValue().size(), 4);
}

test(MultipleOptionsDeserializationTest) {
    unsigned int buffer_size = 7;
    unsigned char buffer[] = {0xb4, 0x74, 0x65, 0x73, 0x74, 0xc1, 0x02};
    unsigned char* buffer_begin = buffer;
    unsigned char* buffer_end = buffer + buffer_size;

    OptionArray option_array;
    CoAPOption::deserialize(buffer_begin, buffer_end, option_array);

    assertEqual(option_array.size(), 2);
    assertEqual(option_array[0].getNumber(), 11);
    assertEqual(option_array[0].getValue().size(), 4);
    assertEqual(option_array[1].getNumber(), 23);
    assertEqual(option_array[1].getValue().size(), 1);
}

test(MultipleOptionsWithPayloadMarkerDeserializationTest) {
    unsigned int buffer_size = 9;
    unsigned char buffer[] = {0xb4, 0x74, 0x65, 0x73, 0x74, 0xc1, 0x02, PAYLOAD_MARKER, 0x00};
    unsigned char* buffer_begin = buffer;
    unsigned char* buffer_end = buffer + buffer_size;

    OptionArray option_array;
    CoAPOption::deserialize(buffer_begin, buffer_end, option_array);

    assertEqual(option_array.size(), 2);
    assertEqual(option_array[0].getNumber(), 11);
    assertEqual(option_array[0].getValue().size(), 4);
    assertEqual(option_array[1].getNumber(), 23);
    assertEqual(option_array[1].getValue().size(), 1);
}

/*test(GetterAndSetterTest) {
    CoAPOption option;

    int expected_ten = 10;
        option.setNumber(expected_ten);
    assertEqual(expected_ten, option.getNumber());
    int expected_hundred = 100;
        option.setNumber(expected_hundred);
    assertEqual(expected_hundred, option.getNumber());
    int expected_thousand = 1000;
        option.setNumber(expected_thousand);
    assertEqual(expected_thousand, option.getNumber());

    ByteArray ten_length;
    ByteArray one_hundred_length;
    ByteArray three_hundred_length;

    for (int i = 0; i < 300; ++i) {
        if (i % 100 == 0) ten_length.pushBack(i);
        if (i % 10 == 0) one_hundred_length.pushBack(i);
        three_hundred_length.pushBack(i);
    }

    option.setValue(ten_length);
    assertEqual(ten_length.size(), option.getLength());
    option.setValue(one_hundred_length);
    assertEqual(one_hundred_length.size(), option.getLength());
    option.setValue(three_hundred_length);
    assertEqual(three_hundred_length.size(), option.getLength());
}*/

test(SingleOptionStringConversionTest) {
    String expected("Test value!");
  
    CoAPOption o(0, expected);
    String actual = o.toString();

    assertEqual(expected, actual);
}

endTest
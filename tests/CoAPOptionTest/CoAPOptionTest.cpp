#include "Test.hpp"

static void assertProperlySerialized(const OptionArray &optionArray, const unsigned char *buffer,
                                     const unsigned int buffer_size) {
    unsigned char actual_buffer[buffer_size];
    unsigned char* actual_buffer_begin = actual_buffer;
    CoAPOption::serialize(actual_buffer_begin, optionArray);

    for (unsigned int i = 0; i < buffer_size; ++i) {
        assertEqual(buffer[i], actual_buffer[i]);
    }
}

beginTest

test(SingleOption) {
    unsigned int buffer_size = 5;
    unsigned char buffer[] = {0xb4, 0x74, 0x65, 0x73, 0x74};
    unsigned char* buffer_begin = buffer;
    unsigned char* buffer_end = buffer + buffer_size;

    OptionArray option_array;
    CoAPOption::deserialize(buffer_begin, buffer_end, option_array);

    assertEqual(option_array.size(), 1);
    assertEqual(option_array[0].getNumber(), 11);
    assertEqual(option_array[0].getValue().size(), 4);

        assertProperlySerialized(option_array, buffer, buffer_size);
}

test(MultipleOptions) {
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

        assertProperlySerialized(option_array, buffer, buffer_size);
}

test(MultipleOptionsWithPayloadMarker) {
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

        assertProperlySerialized(option_array, buffer, buffer_size - 2);
}

test(SingleOptionStringConversion) {
    String expected("Test key!");
  
    CoAPOption o(0, expected);
    String actual = o.toString();

    assertEqual(expected, actual);
}

endTest
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../src/Frame.h"
#include "UdpEmulator.h"
#include "../src/Constants.h"
#include "../src/CoAPHandler.h"

BOOST_AUTO_TEST_CASE(FrameSerializationTest) {
    unsigned int buffer_size = 64;

    Frame frame1;
    unsigned char expected[buffer_size];
    unsigned int bytes_written_expected = frame1.serialize(expected);
    unsigned char actual[buffer_size];
    unsigned int bytes_written_actual = frame1.serialize(actual);

    BOOST_CHECK_EQUAL(bytes_written_expected, bytes_written_actual);
    for (int i = 0; i < bytes_written_expected; ++i) {
        BOOST_CHECK_EQUAL(expected[i], actual[i]);
    }
}

BOOST_AUTO_TEST_CASE(FrameDeserializationTest) {

}

BOOST_AUTO_TEST_CASE(UdpEmulatorTest) {
    UdpEmulator emulator(64);
    unsigned int expected = 1;
    emulator.write(&expected, sizeof(expected));
    unsigned int actual;
    emulator.read(&actual, sizeof(actual));

    BOOST_CHECK_EQUAL(expected, actual);
}

/*BOOST_AUTO_TEST_CASE(EmptyFrameSendingTest) {
    UdpEmulator emulator(64);
    Frame expected;

    emulator.write(&expected, sizeof(expected));

    Frame actual;
    size_t packetSize = emulator.parsePacket() - 8;
    unsigned char* buffer[64];
    emulator.read(&buffer, packetSize);

    BOOST_ASSERT(expected.getVer() == actual.getVer());
    BOOST_ASSERT(expected.getT() == actual.getT());
    BOOST_ASSERT(expected.getTKL() == actual.getTKL());
    BOOST_ASSERT(expected.getCode() == actual.getCode());
    BOOST_ASSERT(expected.getMessageId() == actual.getMessageId());
}*/

/*
BOOST_AUTO_TEST_CASE(SerializationTest) {
    UdpEmulator emulator(64);
    Frame expected;
    expected.setT(0);
    expected.setCode(CODE_GET);
    expected.setMessageId(11);

    unsigned char buffer[64];
    expected.serialize(buffer);

    std::cout << "Buffer: " << buffer << std::endl << "Size: " << sizeof(buffer) << std::endl;
    emulator.write(&expected, sizeof(expected));

    Frame actual;
    size_t packetSize = emulator.parsePacket() - 8;
    emulator.read(&actual, packetSize);
}
*/

/*
BOOST_AUTO_TEST_CASE(FrameSendingTest) {
    UdpEmulator emulator(64);
    Header header = {1,2,3,4,5};
    Frame expected;
    expected.header = header;
    expected.token = new unsigned char[3];
    expected.options = new unsigned char[4];
    expected.payload_marker = Frame::PAYLOAD_MARKER;
    expected.payload = new unsigned char[5];

    emulator.write(&expected, sizeof(expected));

    Frame actual;
    size_t packetSize = emulator.parsePacket() - 8;
    emulator.read(&actual, packetSize);

    std::cout << "Expected: " << expected << std::endl;
    std::cout << "Actual: " << actual << std::endl;
    BOOST_ASSERT(expected.header.Ver == actual.header.Ver);
    BOOST_ASSERT(expected.header.T == actual.header.T);
    BOOST_ASSERT(expected.header.TKL == actual.header.TKL);
    BOOST_ASSERT(expected.header.Code == actual.header.Code);
    BOOST_ASSERT(expected.header.MessageId == actual.header.MessageId);
}*/
/*
BOOST_AUTO_TEST_CASE(GetRequestTest){
    Frame frame;
    frame.setCode(1);
    Option option1;
    option1.setDelta(11);
    option1.setValue((const ByteArray &) "temp");
    OptionArray optionArray(1);
    optionArray.pushBack(option1);
    frame.setOptions(optionArray);
    CoAPHandler coAPHandler;
    coAPHandler.handleMessage(frame);
}
 */


#pragma clang diagnostic pop
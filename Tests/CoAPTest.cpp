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

BOOST_AUTO_TEST_CASE(UdpEmulatorTest) {
    UdpEmulator emulator(64);
    unsigned int expected = 1;
    emulator.write(&expected, sizeof(expected));
    unsigned int actual;
    emulator.read(&actual, sizeof(actual));

    BOOST_CHECK_EQUAL(expected, actual);
}

BOOST_AUTO_TEST_CASE(BasicFrameSendingTest) {
    UdpEmulator emulator(64);
    unsigned char old_buffer[64];
    unsigned char new_buffer[64];
    Frame expected;
    expected.setT(3);
    expected.setCode(CODE_PROXYING_NOT_SUPPORTED);
    expected.setMessageId(65535);

    size_t size = expected.serialize(old_buffer);

    emulator.write(&old_buffer, size);

    size_t packetSize = emulator.parsePacket() - 8;
    emulator.read(&new_buffer, packetSize);

    Frame* actual = Frame::deserialize(new_buffer, packetSize);

    BOOST_ASSERT(expected.getVer() == actual->getVer());
    BOOST_ASSERT(expected.getT() == actual->getT());
    BOOST_ASSERT(expected.getTKL() == actual->getTKL());
    BOOST_ASSERT(expected.getCode() == actual->getCode());
    BOOST_ASSERT(expected.getMessageId() == actual->getMessageId());
    BOOST_ASSERT(actual->getToken().size() == 0);
    BOOST_ASSERT(actual->getOptions().size() == 0);
    BOOST_ASSERT(actual->getPayload().size() == 0);

    delete actual;
}

BOOST_AUTO_TEST_CASE(PayloadFrameSendingTest) {
    UdpEmulator emulator(64);
    unsigned char old_buffer[64];
    unsigned char new_buffer[64];
    Frame expected;
    ByteArray byteArray(5);
    byteArray.pushBack(1);
    byteArray.pushBack(2);
    byteArray.pushBack(3);
    byteArray.pushBack(4);
    byteArray.pushBack(5);
    expected.setPayload(byteArray);

    size_t size = expected.serialize(old_buffer);

    emulator.write(&old_buffer, size);

    size_t packetSize = emulator.parsePacket() - 8;
    emulator.read(&new_buffer, packetSize);

    Frame* actual = Frame::deserialize(new_buffer, packetSize);

    BOOST_ASSERT(actual->getToken().size() == 0);
    BOOST_ASSERT(actual->getOptions().size() == 0);
    BOOST_ASSERT(actual->getPayload()[0] == 1);
    BOOST_ASSERT(actual->getPayload()[1] == 2);
    BOOST_ASSERT(actual->getPayload()[2] == 3);
    BOOST_ASSERT(actual->getPayload()[3] == 4);
    BOOST_ASSERT(actual->getPayload()[4] == 5);

    delete actual;
}

BOOST_AUTO_TEST_CASE(OptionsFrameSendingTest) {
    UdpEmulator emulator(4096);
    unsigned char old_buffer[4096];
    unsigned char new_buffer[4096];
    Frame expected;
    OptionArray optionArray(5);

    for (int i = 0; i < 5; ++i) {
        Option option;
        option.setDelta((unsigned short) (100 * i));

        ByteArray byteArray((unsigned int) (100 * i));
        for (int j = 0; j < 100 * i; ++j) {
            byteArray.pushBack((unsigned char) j);
        }
        option.setValue(byteArray);
        optionArray.pushBack(option);
    }
    expected.setOptions(optionArray);

    size_t size = expected.serialize(old_buffer);

    emulator.write(&old_buffer, size);

    size_t packetSize = emulator.parsePacket() - 8;
    emulator.read(&new_buffer, packetSize);

    Frame* actual = Frame::deserialize(new_buffer, packetSize);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 100 * i; ++j) {
            BOOST_ASSERT(expected.getOptions()[i].getValue()[j] == actual->getOptions()[i].getValue()[j]);
        }
    }

    delete actual;
}

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

BOOST_AUTO_TEST_CASE(SuccessResponseTest){
    Frame test_frame;
    test_frame.setCode(1);
    test_frame.setT(0);
    ByteArray option_value(3);
    option_value.pushBack(1);
    option_value.pushBack(2);
    option_value.pushBack(3);
    Option option1;
    option1.setDelta(3);
    option1.setValue(option_value);
    OptionArray optionArray(1);
    optionArray.pushBack(option1);
    test_frame.setOptions(optionArray);
    CoAPHandler coAPHandler;

    ByteArray payload_value(3);
    payload_value.pushBack('p');
    payload_value.pushBack('a');
    payload_value.pushBack('y');

    Frame response1=coAPHandler.successWrapper(test_frame, payload_value);
    BOOST_CHECK_EQUAL(response1.getT(),2);
    BOOST_CHECK_EQUAL(response1.getCode(),69);
    unsigned char value = response1.getOptions()[0].getValue()[0];
    BOOST_CHECK_EQUAL(value,0);
}



#pragma clang diagnostic pop
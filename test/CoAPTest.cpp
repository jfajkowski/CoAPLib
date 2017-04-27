#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CoAPTest
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../src/Frame.h"
#include "UdpEmulator.h"

BOOST_AUTO_TEST_CASE(HeaderStructureTest) {
    Header header = {1,2,3,4,5};

    BOOST_ASSERT(header.Ver == 1);
    BOOST_ASSERT(header.T == 2);
    BOOST_ASSERT(header.TKL == 3);
    BOOST_ASSERT(header.Code == 4);
    BOOST_ASSERT(header.MessageId == 5);

    BOOST_ASSERT(sizeof(header) == 4);
}

BOOST_AUTO_TEST_CASE(HeaderSendingTest) {
    UdpEmulator emulator(64);
    Header expected = {1,2,3,4,5};

    emulator.write(&expected, sizeof(expected));

    Header actual;
    emulator.read(&actual, sizeof(Header));

    BOOST_ASSERT(expected.Ver == actual.Ver);
    BOOST_ASSERT(expected.T == actual.T);
    BOOST_ASSERT(expected.TKL == actual.TKL);
    BOOST_ASSERT(expected.Code == actual.Code);
    BOOST_ASSERT(expected.MessageId == actual.MessageId);
}
/*
BOOST_AUTO_TEST_CASE(EmptyFrameSendingTest) {
    UdpEmulator emulator(64);
    Header header = {1,2,3,4,5};
    Frame expected;
    expected.header = header;
    expected.token = new unsigned char[0];
    expected.options = new unsigned char[0];
    expected.payload = new unsigned char[0];

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
}

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

#pragma clang diagnostic pop
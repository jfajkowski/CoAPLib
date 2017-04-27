#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CoAPTest
#include <boost/test/unit_test.hpp>
#include "../src/CoAP.h"
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

    emulator.write(&expected);

    Header actual;
    emulator.read(&actual, sizeof(Header));

    BOOST_ASSERT(expected.Ver == actual.Ver);
    BOOST_ASSERT(expected.T == actual.T);
    BOOST_ASSERT(expected.TKL == actual.TKL);
    BOOST_ASSERT(expected.Code == actual.Code);
    BOOST_ASSERT(expected.MessageId == actual.MessageId);
}
#pragma clang diagnostic pop
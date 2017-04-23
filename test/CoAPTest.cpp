#define BOOST_TEST_MODULE CoAPTest
#include <boost/test/unit_test.hpp>
#include "../src/CoAP.h"

BOOST_AUTO_TEST_CASE(CoAPTest) {
    Header header = {1,2,3,4,5};

    BOOST_ASSERT(header.Ver == 1);
    BOOST_ASSERT(header.T == 2);
    BOOST_ASSERT(header.TKL == 3);
    BOOST_ASSERT(header.Code == 4);
    BOOST_ASSERT(header.MessageId == 5);

    BOOST_ASSERT(sizeof(header) == 4);
}
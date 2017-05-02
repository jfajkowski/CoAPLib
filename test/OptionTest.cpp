#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#include <boost/test/unit_test.hpp>
#include "../src/Option.h"

BOOST_AUTO_TEST_CASE(SerializationTest) {
    Option option1;
    option1.setDelta(1);
    option1.setValue(ByteArray(1));

    Option option2;
    option1.setDelta(2);
    option1.setValue(ByteArray(2));

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


#pragma clang diagnostic pop
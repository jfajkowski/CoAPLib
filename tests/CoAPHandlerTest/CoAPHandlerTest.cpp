#include "Test.hpp"

beginTest

test(SuccessResponseTest){
    CoAPMessage test_frame;
    test_frame.setCode(1);
    test_frame.setT(0);
    ByteArray option_value(3);
    option_value.pushBack(1);
    option_value.pushBack(2);
    option_value.pushBack(3);
    CoAPOption option1;
    option1.setDelta(3);
    option1.setValue(option_value);
    OptionArray optionArray(1);
    optionArray.pushBack(option1);
    //test_frame.setOptions(optionArray);
    CoAPHandler coAPHandler;

    ByteArray payload_value(3);
    payload_value.pushBack('p');
    payload_value.pushBack('a');
    payload_value.pushBack('y');

    CoAPMessage response1=coAPHandler.successWrapper(test_frame, payload_value);
    assertEqual(response1.getT(),2);
    assertEqual(response1.getCode(),69);
    unsigned char value = response1.getOptions()[0].getValue()[0];
    assertEqual(value,0);
}

endTest
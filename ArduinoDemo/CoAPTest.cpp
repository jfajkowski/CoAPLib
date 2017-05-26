#include <ArduinoUnit.h>

#include "Frame.h"
#include "UdpEmulator.h"
#include "Constants.h"
#include "CoAPHandler.h"

test(FrameSerializationTest) {
    unsigned int buffer_size = 64;

    Frame frame1;
    unsigned char expected[buffer_size];
    unsigned int bytes_written_expected = frame1.serialize(expected);
    unsigned char actual[buffer_size];
    unsigned int bytes_written_actual = frame1.serialize(actual);

    assertEqual(bytes_written_expected, bytes_written_actual);
    for (int i = 0; i < bytes_written_expected; ++i) {
        assertEqual(expected[i], actual[i]);
    }
}

test(UdpEmulatorTest) {
    UdpEmulator emulator(64);
    unsigned int expected = 1;
    emulator.write(&expected, sizeof(expected));
    unsigned int actual;
    emulator.read(&actual, sizeof(actual));

    assertEqual(expected, actual);
}

test(BasicFrameSendingTest) {
    UdpEmulator emulator(64);
    unsigned char old_buffer[64];
    unsigned char new_buffer[64];
    Frame expected;
    expected.setT(3);
    expected.setCode(CODE_PROXYING_NOT_SUPPORTED);
    expected.setMessageId(65535);

    unsigned int size = expected.serialize(old_buffer);

    emulator.write(&old_buffer, size);

    unsigned int packetSize = emulator.parsePacket() - 8;
    emulator.read(&new_buffer, packetSize);

    Frame* actual = Frame::deserialize(new_buffer, packetSize);

    assertTrue(expected.getVer() == actual->getVer());
    assertTrue(expected.getT() == actual->getT());
    assertTrue(expected.getTKL() == actual->getTKL());
    assertTrue(expected.getCode() == actual->getCode());
    assertTrue(expected.getMessageId() == actual->getMessageId());
    assertTrue(actual->getToken().size() == 0);
    assertTrue(actual->getOptions().size() == 0);
    assertTrue(actual->getPayload().size() == 0);

    delete actual;
}

test(PayloadFrameSendingTest) {
    UdpEmulator emulator(64);
    unsigned char old_buffer[64];
    unsigned char new_buffer[64];
    Frame expected;
    ByteArray byteArray(4);
    byteArray.pushBack('T');
    byteArray.pushBack('E');
    byteArray.pushBack('S');
    byteArray.pushBack('T');
    expected.setPayload(byteArray);

    unsigned int size = expected.serialize(old_buffer);

    emulator.write(&old_buffer, size);

    unsigned int packetSize = emulator.parsePacket() - 8;
    emulator.read(&new_buffer, packetSize);

    Frame* actual = Frame::deserialize(new_buffer, packetSize);

    assertTrue(actual->getToken().size() == 0);
    assertTrue(actual->getOptions().size() == 0);
    assertTrue(actual->getPayload()[0] == 'T');
    assertTrue(actual->getPayload()[1] == 'E');
    assertTrue(actual->getPayload()[2] == 'S');
    assertTrue(actual->getPayload()[3] == 'T');

    delete actual;
}

test(OptionsFrameSendingTest) {
    UdpEmulator emulator(128);
    unsigned char old_buffer[128];
    unsigned char new_buffer[128];
    Frame expected;
    OptionArray optionArray(2);

    for (int i = 0; i < 2; ++i) {
        Option option;
        option.setDelta((unsigned short) (10 * i));

        ByteArray byteArray((unsigned int) (10 * i));
        for (int j = 0; j < 10 * i; ++j) {
            byteArray.pushBack((unsigned char) j);
        }
        option.setValue(byteArray);
        optionArray.pushBack(option);
    }
    expected.setOptions(optionArray);

    unsigned int size = expected.serialize(old_buffer);

    emulator.write(&old_buffer, size);

    unsigned int packetSize = emulator.parsePacket() - 8;
    emulator.read(&new_buffer, packetSize);

    Frame* actual = Frame::deserialize(new_buffer, packetSize);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 10 * i; ++j) {
            assertTrue(expected.getOptions()[i].getValue()[j] == actual->getOptions()[i].getValue()[j]);
        }
    }

    delete actual;
}

test(SuccessResponseTest){
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
    assertEqual(response1.getT(),2);
    assertEqual(response1.getCode(),69);
    unsigned char value = response1.getOptions()[0].getValue()[0];
    assertEqual(value,0);
}

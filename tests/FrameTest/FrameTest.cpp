#include <ArduinoUnit.h>
#include <CoAPLib.h>

#include "UdpEmulator.h"

test(FrameSerializationTest) {
    unsigned int buffer_size = 64;

    Frame frame;
    unsigned char expected[buffer_size];
    unsigned int bytes_written_expected = frame.serialize(expected);
    unsigned char actual[buffer_size];
    unsigned int bytes_written_actual = frame.serialize(actual);

    assertEqual(bytes_written_expected, bytes_written_actual);
    for (int i = 0; i < bytes_written_expected; ++i) {
        assertEqual(expected[i], actual[i]);
    }
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

    unsigned int packetSize = emulator.parsePacket();
    emulator.read(&new_buffer, packetSize);

    Frame actual;
    Frame::deserialize(&actual, new_buffer, packetSize);

    assertEqual(expected.getVer(), actual.getVer());
    assertEqual(expected.getT(), actual.getT());
    assertEqual(expected.getTKL(), actual.getTKL());
    assertEqual(expected.getCode(), actual.getCode());
    assertEqual(expected.getMessageId(), actual.getMessageId());
    assertEqual(actual.getToken().size(), 0);
    assertEqual(actual.getOptions().size(), 0);
    assertEqual(actual.getPayload().size(), 0);
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

    unsigned int packetSize = emulator.parsePacket();
    emulator.read(&new_buffer, packetSize);

    Frame actual;
    Frame::deserialize(&actual, new_buffer, packetSize);

    assertEqual(actual.getToken().size(), 0);
    assertEqual(actual.getOptions().size(), 0);
    assertEqual(actual.getPayload()[0], 'T');
    assertEqual(actual.getPayload()[1], 'E');
    assertEqual(actual.getPayload()[2], 'S');
    assertEqual(actual.getPayload()[3], 'T');
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

    unsigned int packetSize = emulator.parsePacket();
    emulator.read(&new_buffer, packetSize);

    Frame actual;
    Frame::deserialize(&actual, new_buffer, packetSize);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 10 * i; ++j) {
            assertEqual(expected.getOptions()[i].getValue()[j], actual.getOptions()[i].getValue()[j]);
        }
    }
}

test(PingDeserialization) {
	unsigned int buffer_size = 4;
	unsigned char buffer[] = {64, 1, 0, 1};
	
	Frame frame;
	Frame::deserialize(&frame, buffer, buffer_size);
	
	assertEqual(frame.getVer(), 1);
	assertEqual(frame.getT(), 0);
	assertEqual(frame.getTKL(), 0);
	assertEqual(frame.getCode(), 1);
	assertEqual(frame.getMessageId(), 1);
}


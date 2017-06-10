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

test(PingDeserialization) {
	unsigned int buffer_size = 4;
	unsigned char buffer[] = {0x40, 0x00, 0x5e, 0xb7};
	
	Frame frame;
	Frame::deserialize(&frame, buffer, buffer_size);
	
	assertEqual(frame.getVer(), DEFAULT_VERSION);
	assertEqual(frame.getT(), TYPE_CON);
	assertEqual(frame.getTKL(), 0);
	assertEqual(frame.getCode(), CODE_EMPTY);
	assertEqual(frame.getMessageId(), 24247);
}

test(OptionDeserialization) {
    unsigned int buffer_size = 57;
    unsigned char buffer[] = {0x40, 0x01, 0x24, 0x63, 0xb8, 0x73,
                0x68, 0x75, 0x74, 0x64, 0x6f, 0x77, 0x6e, 0xc1, 0x02};

    Frame frame;
    Frame::deserialize(&frame, buffer, buffer_size);

    assertEqual(frame.getVer(), DEFAULT_VERSION);
    assertEqual(frame.getT(), TYPE_CON);
    assertEqual(frame.getTKL(), 0);
    assertEqual(frame.getCode(), CODE_GET);
    assertEqual(frame.getMessageId(), 9315);
    assertEqual(frame.getOptions().size(), 2);
}


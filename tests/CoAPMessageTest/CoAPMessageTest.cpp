#include "Test.hpp"

beginTest

test(PingMessageDeserialization) {
	unsigned int buffer_size = 4;
	unsigned char buffer[] = {0x40, 0x00, 0x5e, 0xb7};
	
	CoAPMessage message;
    message.deserialize(buffer, buffer_size);
	
	assertEqual(message.getVer(), DEFAULT_VERSION);
	assertEqual(message.getT(), TYPE_CON);
	assertEqual(message.getTKL(), 0);
	assertEqual(message.getCode(), CODE_EMPTY);
	assertEqual(message.getMessageId(), 24247);
}

test(OptionsMessageDeserialization) {
    unsigned int buffer_size = 15;
    unsigned char buffer[] = {0x40, 0x01, 0x24, 0x63, 0xb8, 0x73,
                0x68, 0x75, 0x74, 0x64, 0x6f, 0x77, 0x6e, 0xc1, 0x02};

    CoAPMessage message;
    message.deserialize(buffer, buffer_size);

    assertEqual(message.getVer(), DEFAULT_VERSION);
    assertEqual(message.getT(), TYPE_CON);
    assertEqual(message.getTKL(), 0);
    assertEqual(message.getCode(), CODE_GET);
    assertEqual(message.getMessageId(), 9315);
    assertEqual(message.getOptions().size(), 2);
    assertEqual(message.getOptions()[0].getNumber(), 11);
    assertEqual(message.getOptions()[0].getValue().size(), 8);
    assertEqual(message.getOptions()[1].getNumber(), 23);
    assertEqual(message.getOptions()[1].getValue().size(), 1);
}

test(FancyMessageDeserialization) {
    unsigned int buffer_size = 82;
    unsigned char buffer[] = {0x60, 0x45, 0x61, 0x91, 0x48, 0xde, 0xea, 0x9a, 0x3e,
                              0x0e, 0xda, 0xc4, 0x9b, 0x81, 0x28, 0xb1, 0xaa, 0xff,
                              0x74, 0x3d, 0x30, 0x2c, 0x3c, 0x2f, 0x6c, 0x61, 0x72,
                              0x67, 0x65, 0x2d, 0x75, 0x70, 0x64, 0x61, 0x74, 0x65,
                              0x3e, 0x3b, 0x72, 0x74, 0x3d, 0x22, 0x6c, 0x61, 0x72,
                              0x67, 0x65, 0x2d, 0x75, 0x70, 0x64, 0x61, 0x74, 0x65,
                              0x22, 0x3b, 0x63, 0x74, 0x3d, 0x30, 0x2c, 0x3c, 0x2f,
                              0x6c, 0x61, 0x72, 0x67, 0x65, 0x2d, 0x63, 0x72, 0x65,
                              0x61, 0x74, 0x65, 0x3e, 0x3b, 0x72, 0x74, 0x3d, 0x22,
                              0x6c};

        unsigned int expected_payload_size = 64;
        unsigned char expected_payload[] = {0x74, 0x3d, 0x30, 0x2c, 0x3c, 0x2f, 0x6c,
                                            0x61, 0x72, 0x67, 0x65, 0x2d, 0x75, 0x70,
                                            0x64, 0x61, 0x74, 0x65, 0x3e, 0x3b, 0x72,
                                            0x74, 0x3d, 0x22, 0x6c, 0x61, 0x72, 0x67,
                                            0x65, 0x2d, 0x75, 0x70, 0x64, 0x61, 0x74,
                                            0x65, 0x22, 0x3b, 0x63, 0x74, 0x3d, 0x30,
                                            0x2c, 0x3c, 0x2f, 0x6c, 0x61, 0x72, 0x67,
                                            0x65, 0x2d, 0x63, 0x72, 0x65, 0x61, 0x74,
                                            0x65, 0x3e, 0x3b, 0x72, 0x74, 0x3d, 0x22,
                                            0x6c};


    CoAPMessage message;
    message.deserialize(buffer, buffer_size);

    assertEqual(message.getVer(), DEFAULT_VERSION);
    assertEqual(message.getT(), TYPE_ACK);
    assertEqual(message.getTKL(), 0);
    assertEqual(message.getCode(), CODE_CONTENT);
    assertEqual(message.getMessageId(), 24977);
    assertEqual(message.getOptions().size(), 3);
    assertEqual(message.getOptions()[0].getNumber(), 4);
    assertEqual(message.getOptions()[0].getValue().size(), 8);
    assertEqual(message.getOptions()[1].getNumber(), 12);
    assertEqual(message.getOptions()[1].getValue().size(), 1);
    assertEqual(message.getOptions()[2].getNumber(), 23);
    assertEqual(message.getOptions()[2].getValue().size(), 1);
    assertEqual(message.getPayload().size(), expected_payload_size);

    for (int i = 0; i < message.getPayload().size(); ++i) {
        assertEqual(message.getPayload()[i], expected_payload[i]);
    }
}

test(AddOptionTest) {
    CoAPMessage message;
    CoAPOption first(1, "FIRST");
    CoAPOption second(100, "SECOND");
    CoAPOption third(1000, "THIRD");
    CoAPOption fourth(10000, "FOURTH");

    message.addOption(second);
    message.addOption(fourth);
    message.addOption(third);
    message.addOption(first);

    assertEqual(message.getOptions()[0].getNumber(), first.getNumber());
    assertEqual(message.getOptions()[0].getValue().size(), first.getValue().size());
    assertEqual(message.getOptions()[1].getNumber(), second.getNumber());
    assertEqual(message.getOptions()[1].getValue().size(), second.getValue().size());
    assertEqual(message.getOptions()[2].getNumber(), third.getNumber());
    assertEqual(message.getOptions()[2].getValue().size(), third.getValue().size());
    assertEqual(message.getOptions()[3].getNumber(), fourth.getNumber());
    assertEqual(message.getOptions()[3].getValue().size(), fourth.getValue().size());
}

endTest

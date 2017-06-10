#include "Test.hpp"

beginTest

test(PingMessageDeserialization) {
	unsigned int buffer_size = 4;
	unsigned char buffer[] = {0x40, 0x00, 0x5e, 0xb7};
	
	CoAPMessage message;
    CoAPMessage::deserialize(&message, buffer, buffer_size);
	
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
    CoAPMessage::deserialize(&message, buffer, buffer_size);

    assertEqual(message.getVer(), DEFAULT_VERSION);
    assertEqual(message.getT(), TYPE_CON);
    assertEqual(message.getTKL(), 0);
    assertEqual(message.getCode(), CODE_GET);
    assertEqual(message.getMessageId(), 9315);
    assertEqual(message.getOptions().size(), 2);
    assertEqual(message.getOptions()[0].getDelta(), 11);
    assertEqual(message.getOptions()[0].getLength(), 8);
    assertEqual(message.getOptions()[1].getDelta(), 12);
    assertEqual(message.getOptions()[1].getLength(), 1);
}

endTest

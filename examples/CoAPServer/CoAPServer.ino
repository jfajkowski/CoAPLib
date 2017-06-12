#include <CoAPLib.h>

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <RF24Network.h>
#include <RF24.h>


unsigned const int MAX_BUFFER = 128;
unsigned char packet_buffer[MAX_BUFFER];
byte mac[] = {222, 222, 222, 222, 222, 222};
IPAddress ip(192, 168, 0, 2);
const uint16_t this_node_id = 00;
const uint16_t peer_node_id = 01; //TODO: set id's dynamically?
const uint16_t channel = 100;
RF24NetworkHeader header(peer_node_id);
EthernetUDP Udp;
RF24 radio(7, 8);
RF24Network network(radio);
ServerCoAPHandler coAPHandler;
unsigned short local_port = 10000;

void setup() {
    Serial.begin(9600);
    Ethernet.begin(mac, ip);
    Serial.println(Ethernet.localIP());
    Udp.begin(local_port);
    radio.begin();
    network.begin(channel, this_node_id);
}

void loop() {
    network.update();
    while (network.available()) {
        RadioMessage receivedMessage;
        network.read(header, &receivedMessage, sizeof(receivedMessage));
        coAPHandler.createResponse(receivedMessage);
    }


    CoAPMessage* coAPMessage = coAPHandler.popCoAPMessageToSend();
    RadioMessage* radioMessage = coAPHandler.popRadioMessageToSend();

    if(coAPMessage != nullptr) {
        sendCoAPMessage(*coAPMessage);
        delete coAPMessage;
    }
    if(radioMessage != nullptr) {
        sendRadioMessage(*radioMessage);
        delete radioMessage; //TODO: will it prevent from memory leaks and seg faults?
    }


    unsigned int packet_size = Udp.parsePacket();
    if (packet_size) {
        CoAPMessage message;
        message.deserialize(packet_buffer, packet_size);
        coAPHandler.handleMessage(message);

        Serial.print("Received: ");
        Serial.print(packet_size);
        Serial.println(" bytes.");
        Udp.read(packet_buffer, MAX_BUFFER);

        for (int i = 0; i < packet_size; ++i) {
            Serial.print(packet_buffer[i]);
            Serial.print(" ");
        }

        Serial.println();
    }
}

void sendCoAPMessage(CoAPMessage &message) { //TODO: test
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    unsigned char buffer[MAX_BUFFER];
    unsigned int size = message.serialize(buffer);
    Udp.write(buffer, size);
    Udp.endPacket();
}

void sendRadioMessage(RadioMessage &message) { //TODO: test
    network.write(header, &message, sizeof(message));
}
#include <CoAPLib.h>
#include <RadioLib.h>

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

static struct OnCoAPMessageToSend : public CoAPMessageListener {
    void operator()(const CoAPMessage &message) override {
        message.print();
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        unsigned char buffer[MAX_BUFFER];
        unsigned int size = message.serialize(buffer);
        Udp.write(buffer, size);
        Udp.endPacket();
    }
} onCoAPMessageToSend;

static struct OnRadioMessageToSend : public RadioMessageListener {
    void operator()(const RadioMessage &message) override {
        network.write(header, &message, sizeof(message));
    }
} onRadioMessageToSend;

CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
unsigned short local_port = 10000;

unsigned short lamp_value = 1;
unsigned short speaker_value = 1;

void setup() {
    Ethernet.begin(mac, ip);
    Serial.println(Ethernet.localIP());
    Udp.begin(local_port);
    radio.begin();
    network.begin(channel, this_node_id);

    Serial.begin(9600);
    Serial.println("Rise and shine...");
}

void loop() {
/*
    Serial.print("LAMP: ");
    Serial.println(lamp_value);
    onRadioMessageToSend({0, PUT, LAMP, lamp_value++});
    Serial.println("Loop beginning");
    network.update();
    while (network.available()) {
       }

    delay(1000);

    Serial.print("SPEAKER: ");
    Serial.println(speaker_value);
    onRadioMessageToSend({0, PUT, SPEAKER, speaker_value++});

    delay(1000);

    if (lamp_value == 255)
        lamp_value = 1;
    if (speaker_value == 255)
        speaker_value = 1;
*/
//    network.update();
//    while (network.available()) {
//        RadioMessage receivedMessage;
//        network.read(header, &receivedMessage, sizeof(receivedMessage));
//        Serial.print("Radio message read");
//        coAPHandler.createResponse(receivedMessage);
//        Serial.print("Created response to radio message");
//    }

    unsigned int packet_size = Udp.parsePacket();
    if (packet_size) {
        Serial.print("Received: ");
        Serial.print(packet_size);
        Serial.println(" bytes.");
        Udp.read(packet_buffer, MAX_BUFFER);

        for (int i = 0; i < packet_size; ++i) {
            Serial.print(packet_buffer[i]);
            Serial.print(" ");
        }
        Serial.println();

        CoAPMessage message;
        message.deserialize(packet_buffer, packet_size);
        message.print();
        coAPHandler.handleMessage(message);

    }
}
#include <SPI.h>

#include <Ethernet.h>
#include <EthernetUdp.h>

#include <RF24Network.h>
#include <RF24.h>

#include <CoAPLib.h>
#include <RadioLib.h>

// Ethernet library variables:
IPAddress invalid(0, 0, 0, 0);
unsigned short local_port = 10000;
byte mac[] = {222, 222, 222, 222, 222, 222};
EthernetUDP Udp;
unsigned const int MAX_BUFFER = 64;
unsigned char packet_buffer[MAX_BUFFER];

// RF24 library variables:
const uint16_t this_node_id = 00;
const uint16_t peer_node_id = 01; //TODO: set id's dynamically?
const uint16_t channel = 100;
RF24NetworkHeader header(peer_node_id);
RF24 radio(7, 8);
RF24Network network(radio);

// CoAPLib variables:
struct : public CoAPMessageListener {
    void operator()(const CoAPMessage &message) {
        unsigned int size = message.serialize(packet_buffer);
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(packet_buffer, size);
        Udp.endPacket();
    }
} onCoAPMessageToSend;

struct : public RadioMessageListener {
    void operator()(const RadioMessage &message) {
        network.write(header, &message, sizeof(message));
    }
} onRadioMessageToSend;

CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
unsigned short ping_interval = 10000;
unsigned long last_ping_sent = 0;
unsigned long last_timeout_check = 0;

void setup() {
    Serial.begin(115200);

    // Prepare Ethernet:
    Ethernet.begin(mac);                    // Connected to router
    Serial.println(Ethernet.localIP());
    Udp.begin(local_port);

    // Prepare RF24:
    radio.begin();
    network.begin(channel, this_node_id);

    Serial.println("Rise and shine...");
}

void loop() {
    network.update();
    while (network.available()) {
        RadioMessage receivedMessage;
        network.read(header, &receivedMessage, sizeof(receivedMessage));
        coAPHandler.handleMessage(receivedMessage);
    }

    unsigned int packet_size = Udp.parsePacket();

    if (packet_size) {
        Udp.read(packet_buffer, MAX_BUFFER);

        DEBUG_PRINT("Received ");
        DEBUG_PRINT(packet_size);
        DEBUG_PRINT(" bytes: ");

        for (int i = 0; i < packet_size; ++i) {
            DEBUG_PRINTHEX(packet_buffer[i]);
            DEBUG_PRINT(" ");
        }
        DEBUG_PRINTLN();

        CoAPMessage message;
        message.deserialize(packet_buffer, packet_size);
        coAPHandler.handleMessage(message);
    }

//    unsigned long now = millis();
//    if (now - last_timeout_check >= coAPHandler.getTimeout()) {
//        coAPHandler.deleteTimedOut();
//        last_timeout_check = now;
//    }
//    if (now - last_ping_sent >= ping_interval) {
//
//        if (Udp.remoteIP() != invalid) {
//            DEBUG_PRINT_TIME();
//            DEBUG_PRINT("PING ");
//            DEBUG_PRINT(Udp.remoteIP());
//            DEBUG_PRINT(":");
//            DEBUG_PRINTLN(Udp.remotePort());
//            coAPHandler.sendPing();
//            last_ping_sent = now;
//        }
//    }
}

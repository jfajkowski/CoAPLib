#include <SPI.h>

#include <Ethernet.h>
#include <EthernetUdp.h>

#include <RF24Network.h>
#include <RF24.h>

#include <CoAPLib.h>
#include <RadioLib.h>

// Ethernet library variables:
IPAddress ip(192, 168, 0, 2);                   // Used only when directly connected.
unsigned short local_port = 10000;
byte mac[] = {222, 222, 222, 222, 222, 222};
EthernetUDP Udp;
unsigned const int MAX_BUFFER = 128;
unsigned char packet_buffer[MAX_BUFFER];

// RF24 library variables:
const uint16_t this_node_id = 00;
const uint16_t peer_node_id = 01; //TODO: set id's dynamically?
const uint16_t channel = 100;
RF24NetworkHeader header(peer_node_id);
RF24 radio(7, 8);
RF24Network network(radio);

// Callbacks for CoAPHandler:
struct : public CoAPMessageListener {
    void operator()(const CoAPMessage &message) {
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        unsigned char buffer[MAX_BUFFER];
        unsigned int size = message.serialize(buffer);
        Udp.write(buffer, size);
        Udp.endPacket();
    }
} onCoAPMessageToSend;

struct : public RadioMessageListener {
    void operator()(const RadioMessage &message) {
        network.write(header, &message, sizeof(message));
    }
} onRadioMessageToSend;

CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);

void setup() {
    Serial.begin(9600);

    // Prepare Ethernet:
    Ethernet.begin(mac);                    // Connected to router
    //Ethernet.begin(mac, ip);              // Directly connected
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
        DEBUG_PRINT("Received: ");
        DEBUG_PRINT(packet_size);
        DEBUG_PRINTLN(" bytes.");
        Udp.read(packet_buffer, MAX_BUFFER);

        for (int i = 0; i < packet_size; ++i) {
            DEBUG_PRINT(packet_buffer[i]);
            DEBUG_PRINT(" ");
        }
        DEBUG_PRINTLN();

        CoAPMessage message;
        message.deserialize(packet_buffer, packet_size);
        message.print();
        coAPHandler.handleMessage(message);
    }
}
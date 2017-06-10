#include <CoAPLib.h>

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

unsigned const int MAX_BUFFER = 128;
unsigned char packet_buffer[MAX_BUFFER];
byte mac[]={222, 222, 222, 222, 222, 222};
EthernetUDP Udp;
unsigned short local_port = 10000;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  Serial.println(Ethernet.localIP());
  Udp.begin(local_port);
}

void loop() {
  unsigned int packet_size=Udp.parsePacket();
  if(packet_size) {
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

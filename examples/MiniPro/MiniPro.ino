#include <SPI.h>
#include <RF24Network.h>
#include <RF24.h>
#include <RadioLib.h>

RF24 radio(7, 8);                     // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);           // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format


unsigned short lampValue;
unsigned short speakerValue;
unsigned long interval = 0; //ms
unsigned long last_sent;
bool runningFlag = false;

RadioMessage message;


void setup() {
    lampValue = 255;
    speakerValue = 0;
    radio.begin();
    network.begin(/*channel*/ 100, /*node address*/ this_node);

    pinMode(3, OUTPUT);
    pinMode(2, OUTPUT);

    setLampBrightness(lampValue);
    setSpeakerFrequency(speakerValue);

    Serial.begin(115200);
    Serial.println("Rise and shine...");
}

void loop() {
    network.update();                          // Check the network regularly

    lampValue = analogRead(3);
    unsigned long now = millis();

    if (runningFlag && now - last_sent >= interval) {
        last_sent = now;
        digitalWrite(2, digitalRead(2) ^ 1);
    }

    while (network.available()) {     // Is there anything ready for us?
        RF24NetworkHeader header;
        network.read(header, &message, sizeof(message));

        if(message.resource == RADIO_LAMP){
            if(message.code == RADIO_PUT)
                setLampBrightness(message.value);
            send(convert(lampValue));
        }
        else if(message.resource == RADIO_SPEAKER){
            if(message.code == RADIO_PUT)
                setSpeakerFrequency(message.value);
            send(speakerValue);
        }
    }
}

void setLampBrightness(unsigned short level){
    int tmp = (int) level/4;
    tmp = tmp - 250;
    tmp = tmp * -1;
    tmp = tmp + 5;
    lampValue = tmp;
    analogWrite(3, lampValue);
}

void setSpeakerFrequency(unsigned short freq){
    speakerValue = freq;

    if (speakerValue > 0){
        runningFlag = true;
        interval = (1.0/speakerValue) * 1000;
    }
    else runningFlag = false;
}

unsigned short convert(unsigned short val){
    int tmp = (int) val;
    tmp = tmp - 255;
    tmp = tmp * -4;
    return (unsigned short) tmp;
}

void send(unsigned short value){
    message.value = value;
    RF24NetworkHeader header(/*to node*/ other_node);
    network.write(header, &message, sizeof(message));
}
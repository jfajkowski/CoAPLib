#include <SPI.h>
#include <RF24Network.h>
#include <RF24.h>
#include <RadioLib.h>

RF24 radio(7, 8);                       // nRF24L01(+) radio

RF24Network network(radio);             // Network initialized with radio

const uint16_t this_node_id = 01;       // Address of our node in Octal format
const uint16_t other_node_id = 00;      // Address of the other node in Octal format
const uint16_t channel = 100;

unsigned short lampValue;               // Brightness of lamp, 255-0, where 0 is maximum brightness
unsigned short speakerValue;            // Frequency of speaker, 0 means speaker is off
unsigned long interval = 0;             // Speaker buzzing interval in milliseconds
unsigned long last_sent;                // Last time the speaker buzzed
bool runningFlag = false;               // Indicates if buzzer is working

RadioMessage message;


void setup() {
    lampValue = 255;                    // Set lamp value to minimum
    speakerValue = 0;                   // Set speaker value to off
    radio.begin();
    network.begin(channel, this_node_id);

    pinMode(3, OUTPUT);
    pinMode(2, OUTPUT);

    setLampBrightness(lampValue);       // Initialize lamp with min value
    setSpeakerFrequency(speakerValue);  // Initialize speaker with min value

    Serial.begin(9600);
    Serial.println("Rise and shine...");
}

void loop() {
    network.update();                   // Check the network regularly

    lampValue = analogRead(3);          // Read current lamp value
    unsigned long now = millis();

    // After every interval buzz the speaker
    if (runningFlag && now - last_sent >= interval) {
        last_sent = now;
        digitalWrite(2, digitalRead(2) ^ 1);
    }

    // Check if there is anything ready to receive
    while (network.available()) {
        RF24NetworkHeader header;
        network.read(header, &message, sizeof(message));
        Serial.println("Received:");
        message.print();

        // Depending on action update, or just send current values
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

// Set lamp brightness using 0-1000 level of brightness range.
// Converts 0-1000 into 5-255. Levels 0-4 are 5 top levels and humans can't see the difference anyway
void setLampBrightness(unsigned short level){
    int tmp = (int) level/4;
    tmp = tmp - 250;
    tmp = tmp * -1;
    tmp = tmp + 5;
    lampValue = tmp;
    analogWrite(3, lampValue);
}

// Set speaker frequency
void setSpeakerFrequency(unsigned short freq){
    speakerValue = freq;

    if (speakerValue > 0) {
        runningFlag = true;
        interval = (1.0/speakerValue) * 1000;
    }
    else runningFlag = false;
}

// Convert 5-255 value range to human input range
unsigned short convert(unsigned short val){
    int tmp = (int) val;
    tmp = tmp - 255;
    tmp = tmp * -4;
    return (unsigned short) tmp;
}

// Send Radio Message
void send(unsigned short value){
    message.value = value;
    Serial.println("Sent:");
    message.print();
    RF24NetworkHeader header(other_node_id);
    network.write(header, &message, sizeof(message));
}
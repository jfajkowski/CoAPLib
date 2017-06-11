#include <RF24Network.h>
#include <RF24.h>



RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format


unsigned char lampValue : 14;
unsigned char speakerValue: 14;
unsigned long interval = 0; //ms
unsigned long last_sent;
bool runningFlag=false;

struct RadioMessage{
unsigned int message_id: 16;
unsigned char code: 1;
unsigned char resource: 1;
unsigned char value: 14;
} message;


void setup() {
  lampValue = 255;
  speakerValue = 0;
  radio.begin();
  network.begin(/*channel*/ 100, /*node address*/ this_node);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);  //pewnie nie będzie się to nazywać A1, skoro potrzebujemy digitala

}

void loop() {
  network.update();                          // Check the network regularly

  lampValue = analogRead(A0);
  unsigned long now = millis();

  if (runningFlag && now - last_sent >= interval  )
    {
      last_sent = now;
      digitalWrite(A1,digitalRead(9) ^ 1);
    }

  while ( network.available() ) {     // Is there anything ready for us?

    RF24NetworkHeader header;
    network.read(header,&message,sizeof(message));
    if(message.resource==0){
      if(message.code==0)
        setLamp(message.value);
      send(lampValue);
    }
    else if(message.resource==1){
      if(message.code==0)
        setSpeakerFrequency(message.value);
      send(speakerValue);
    }
  }
}

void setLamp(unsigned char level ){
    lampValue=level;
    analogWrite(A0,lampValue);

}

void send(unsigned char value:14){
    message.value=value;
    RF24NetworkHeader header(/*to node*/ other_node);
    network.write(header,&message,sizeof(message));
}

void setSpeakerFrequency(unsigned char freq){
    speakerValue=message.value;
    if (speakerValue>0){
      runningFlag=true;
    }
    else runningFlag=false;
    interval = (1/(double)speakerValue)*1000;
}



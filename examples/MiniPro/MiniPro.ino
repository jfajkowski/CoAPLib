#include <RF24Network.h>
#include <RF24.h>
#include <RadioLib.h>




RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format


unsigned short lampValue;
unsigned short speakerValue;
unsigned long interval = 0; //ms
unsigned long last_sent;
bool runningFlag=false;

RadioMessage message;


void setup() {
  lampValue = 255;
  speakerValue = 50;
  setSpeakerFrequency(speakerValue);
  radio.begin();
  network.begin(/*channel*/ 100, /*node address*/ this_node);

  pinMode(A0, OUTPUT);
  pinMode(2, OUTPUT);  //pewnie nie będzie się to nazywać A1, skoro potrzebujemy digitala

}

void loop() {
  network.update();                          // Check the network regularly

  lampValue = analogRead(A0);
  unsigned long now = millis();

  if (runningFlag && now - last_sent >= interval  )
    {
      last_sent = now;
      digitalWrite(2,digitalRead(2) ^ 1);
    }

  while ( network.available() ) {     // Is there anything ready for us?

    RF24NetworkHeader header;
    network.read(header,&message,sizeof(message));
    if(message.resource==0){
      if(message.code==0)
        setLamp(message.value);
      send(convert(lampValue));
    }
    else if(message.resource==1){
      if(message.code==0)
        setSpeakerFrequency(message.value);
      send(speakerValue);
    }
  }
}

void setLamp(unsigned char level ){
    int temp=(int)level/4;
    temp=temp -250;
    temp=temp*-1;
    temp=temp+5;
    level =(unsigned short)temp;
    lampValue=level;
    analogWrite(A0,lampValue);

}
unsigned short convert(unsigned short val){
    int temp=(int)val;
    temp=temp-255;
    temp=temp*-4;
    return (unsigned short)temp;
}

void send(unsigned short value){
    message.value=value;
    RF24NetworkHeader header(/*to node*/ other_node);
    network.write(header,&message,sizeof(message));
}

void setSpeakerFrequency(unsigned short freq){
    speakerValue=message.value;
    if (speakerValue>0){
      runningFlag=true;
    }
    else runningFlag=false;
    interval = (1/(double)speakerValue)*1000;
}



#include <ArduinoUnit.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  Test::run();
}
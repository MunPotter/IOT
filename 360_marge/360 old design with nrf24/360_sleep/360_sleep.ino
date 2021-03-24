
#include "Arduino.h"

#define wakePin 16


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(wakePin, INPUT);//try pull down too
  Serial.println("Setup completed.");
}

void loop() {
  Going_to_sleep();
}

void blinkMe() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

void Going_to_sleep() {
  blinkMe();
  blinkMe();
  ESP.deepSleep(0);
}

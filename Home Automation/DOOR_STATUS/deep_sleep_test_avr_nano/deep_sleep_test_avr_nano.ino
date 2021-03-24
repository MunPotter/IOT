#include "Arduino.h"
#include <avr/sleep.h>
#include "avr/io.h"
#include "avr/interrupt.h"

#define wakePin PB1


void setup() {
  Serial.begin(9600);
  pinMode(wakePin, INPUT);
  Serial.println("onSetup function.");
}

void loop() {
  delay(100);
  Going_to_sleep();
}

void Going_to_sleep() {
  Serial.print("digital pin 2 status: ");
  Serial.println(digitalRead(2));
  delay(500);
  if (digitalRead(wakePin) == LOW) {
    Serial.println("Going to sleep from digitalPin 2 = LOW");
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    noInterrupts();
    attachInterrupt(digitalPinToInterrupt(wakePin), Wake_up, HIGH);
  }
  if (digitalRead(wakePin) == HIGH) {
    Serial.println("Going to sleep from digitalPin 2 = HIGH");
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    noInterrupts();
    attachInterrupt(digitalPinToInterrupt(wakePin), Wake_up, LOW);
  }
  Serial.println("Going to sleep Good Bye!");
  Serial.flush();
  interrupts();
  sleep_cpu();
  Serial.println("I'm awake!");
}
void Wake_up() {
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(wakePin));
}

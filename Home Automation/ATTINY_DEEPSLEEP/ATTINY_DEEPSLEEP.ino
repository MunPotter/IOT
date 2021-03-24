#include <avr/interrupt.h>
#include <avr/sleep.h>
const int builtInLED = 0;            // LED connected to digital pin 0
const int interrupLED = 4;      // LED to show the action of a interrupt
//const int intStatusLED = 5;
const int signalPin = 2;            // active LOW, ground this pin momentary to wake up

void blinkG() {
  digitalWrite(builtInLED, HIGH);
  delay(100);
  digitalWrite(builtInLED, LOW);
  delay(100);
}
void blinkR() {
  digitalWrite(interrupLED, HIGH);
  delay(100);
  digitalWrite(interrupLED, LOW);
  delay(100);
}

void setup() {
  pinMode(builtInLED, OUTPUT);
  pinMode(interrupLED, OUTPUT);
  pinMode(signalPin, INPUT);
  blinkR();  blinkG();  blinkR();  blinkG();
}

void wakeUpNow() {
  detachInterrupt(0);
  blinkG(); blinkG(); blinkG(); blinkG(); blinkG();
}

void sleepNow() {
  delay(1000);
  blinkR(); blinkR(); blinkR();
  blinkG(); blinkG(); blinkG();
  delay(1000);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  if (digitalRead(signalPin) == LOW) {
    blinkG(); delay(200); blinkG(); delay(200); blinkG(); delay(200);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    attachInterrupt(0, wakeUpNow, LOW);
    sleep_mode();

    sleep_disable();
  }
  if (digitalRead(signalPin) == HIGH) {
    blinkR(); delay(200); blinkR(); delay(200); blinkR(); delay(200);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    attachInterrupt(0, wakeUpNow, LOW);
    sleep_mode();

    sleep_disable();
  }


}

void loop() {
  delay(1000);
  blinkR(); blinkR(); blinkR(); blinkR(); blinkR();
  delay(1000);
  sleepNow();
}

#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


int sensorPin = 1;//REED SWITCH
int EspStartPin = 2;//ESP POWER


void setup() {

  pinMode(EspStartPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(0, OUTPUT);

  sbi(GIMSK, PCIE);
  sbi(PCMSK, PCINT1);
}

void system_sleep() {
  cbi(ADCSRA, ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  sbi(ADCSRA, ADEN);
}
ISR(PCINT0_vect) {
}

void blinkMe() {
  digitalWrite(0, HIGH);
  delay(200);
  digitalWrite(0, LOW);
  delay(200);
}

void loop() {
  if (digitalRead(sensorPin) == HIGH || digitalRead(sensorPin) == LOW ) {
    digitalWrite(EspStartPin, HIGH);
    delay(50);
    digitalWrite(EspStartPin, LOW);
    delay(200);
    blinkMe(); blinkMe(); blinkMe();
    system_sleep();
  }
}

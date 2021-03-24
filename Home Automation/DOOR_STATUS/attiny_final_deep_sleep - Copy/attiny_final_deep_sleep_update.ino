#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


int sensorPin = 1;//REED SWITCH
int EspStartPin = 2;//ESP POWER
int DonePin = 4;//ESP DONE




void setup() {

  pinMode(EspStartPin, OUTPUT);
  pinMode(DonePin, INPUT_PULLUP);
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
  delay(500);
  digitalWrite(0, LOW);
  delay(500);
}

void loop() {
  int counter = 0;

  if (digitalRead(sensorPin) == HIGH || digitalRead(sensorPin) == LOW ) {
    digitalWrite(EspStartPin, HIGH);
  }

  blinkMe(); blinkMe(); blinkMe();
  delay(5000);
  blinkMe(); blinkMe(); blinkMe();
  

  while (digitalRead(DonePin) == HIGH  && counter <= 20) {
    delay(1000);
    counter += 1;
    //    if (digitalRead(DonePin) == HIGH || counter == 100) {
    //      digitalWrite(EspStartPin, LOW);3333333333
    //    }
  }
  digitalWrite(EspStartPin, LOW);

  system_sleep();
}

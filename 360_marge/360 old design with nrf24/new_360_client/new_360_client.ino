//sleep library
#include "Arduino.h"
#include <avr/sleep.h>
//nrf library
#include <SPI.h>
#include <RH_NRF24.h>

#define sensorIN 3 //reed sensor 
#define wakePin 2 // deep sleep pin


//voltage variable
int battVolts;

int sensorValue = 0; //sensor variable

RH_NRF24 nrf24(9, 10); //NRF CE and CSN pin


void setup()
{
  //  CLKPR = 0x80;
  //  CLKPR = 0x02;
  //  Serial.begin(38400);
  Serial.begin(9600);

  //   deep sleep setup
  //  pinMode(sensorIN, INPUT_PULLUP);   // COMMON ROUTE

  pinMode(sensorIN, INPUT);
  digitalWrite(sensorIN, LOW);
  pinMode(wakePin, INPUT_PULLUP);


  //  nrf initialization
  if (!nrf24.init())
    Serial.println("init failed");
  if (!nrf24.setChannel(2))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}


void loop() {
  //reed sensor data collection
  sensorValue = digitalRead(sensorIN);
  Serial.print("sensor value: ");
  Serial.println(sensorValue);


  //battery voltage collection
  battVolts = getBandgap();
  Serial.print("Battery Vcc volts =  ");
  Serial.println(battVolts);

  //preparing data to send server
  uint8_t data[2];
  data[0] = battVolts;
  data[1] = sensorValue;
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
  Serial.print("s value:");
  Serial.println(data[0]);

  //   store data from server
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  //sleep condition
  if (nrf24.recv(buf, &len))
  {
    Serial.print("got reply from server : ");
    delay(300);

    String a = "sleep";
    String b = (char*)buf;
    if (a == b) {
      Serial.println(b);
      Going_To_Sleep();
    }
    else {
      Serial.print("cap open, please continue");
    }
  }
}


// deep sleep instruction
void Going_To_Sleep() {

  //    if (digitalRead(sensorIN) == LOW) {  //  COMMON ROUTE

  if (digitalRead(sensorIN) == HIGH) {
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    noInterrupts();
    attachInterrupt(digitalPinToInterrupt(wakePin), Wake_up, LOW);
    Serial.println("Going to sleep");
    delay(2000);
    Serial.println("Good Bye!");
    Serial.flush();
    interrupts();
    sleep_cpu();
    Serial.println("I'm awake!");
  }

}


void Wake_up() {
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(wakePin));
}

int getBandgap(void) 
{
  const long InternalReferenceVoltage = 1056L;  
  ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
  delay(50); 
  ADCSRA |= _BV( ADSC );
  while ( ( (ADCSRA & (1 << ADSC)) != 0 ) );
  int results = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L; 
  return results;
}

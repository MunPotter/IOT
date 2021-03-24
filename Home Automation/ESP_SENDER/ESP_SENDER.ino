/*  ===========================================================================
       Option (2):
                       ESP12
                      --------------
                     | RST       TX |
                     | ADC       RX |
            (VCC)--->| CH        05 |
                     | 16        04 |
                     | 14        00 |
     switch value--->| 12        02 |
             done--->| 13        15 |<--GND
            (VCC)--->| V        GND |<--Collector
                      --------------

  ============================================================================= */


#include "ESP8266WiFi.h"
#include "espnow.h"



#define DonePin   13
#define SwitchPin 12


int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message {
  uint8_t sensor_id[6];
  int sensor_status;
  float battery_voltage;
} struct_message;

struct_message msg;
ADC_MODE(ADC_VCC);

void setup() {

  Serial.begin(115200);
  pinMode(DonePin, OUTPUT);
  digitalWrite(DonePin, HIGH);
  sendReading();        // send initial state when booted
  OnDataSent();
}

void loop() {
  delay(random(100, 7000));
  sendReading();
}

void sendReading() {
  msg.sensor_status = digitalRead(SwitchPin);
  msg.sensor_status = random(0, 2);
  msg.battery_voltage = ((float)ESP.getVcc()) / 1024;
  Serial.println("Button actioned");
  //  esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
}


// Callback when data is sent
void OnDataSent() {
  //void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {

  Serial.print(msg.sensor_status);
  Serial.print(" [");
  Serial.print(msg.battery_voltage);
  Serial.print("v]");
  Serial.println();

  gotoSleep();
}

void gotoSleep() {
  digitalWrite(DonePin, LOW);
  delay(10);
  ESP.deepSleep(0);      // deep sleep
}

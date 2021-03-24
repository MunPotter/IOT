
#include <Ticker.h>
#include "ReedSensor.h"
#include "JsonData.h"
#include "HttpRequest.h"
#include "WifiConfig.h"
#include "Eeprom.h"

Ticker Tick;

int BUTTON = 2;
int LED = 0;
int REED = 3;

int counter = 0;
volatile int WatchDogCount = 0;
float battery_voltage;

ADC_MODE(ADC_VCC);

void ISRWatchDog() {
  WatchDogCount++;
  if (WatchDogCount > 200000) {
    eprom.epromWriteChar(200, '1');
    ESP.reset();
  }
}

void setup() {
  WatchDogCount = 0;
  pinMode(LED, OUTPUT);
  pinMode(REED, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);
  EEPROM.begin(512);
  Tick.attach(1, ISRWatchDog);
  wifi.wifiSetup();

  String m = WiFi.macAddress();
  String ssid = WiFi.SSID();
  String ip = WiFi.localIP().toString();
  String door = reed.reedData(REED);

  battery_voltage = ((float)ESP.getVcc()) / 1024;
  String f = "";
  f += door; f += "@"; f += battery_voltage; f += "@"; f += m; f += "@"; f += ssid; f += "@"; f += ip;

  String SensorData = jsonData.jsonDataString(f);
  
  char eeprom = eprom.epromReadChar(200);
  if (eeprom == '0') {
    httpRequest.SendToServer(SensorData);
  }



  if (!digitalRead(BUTTON))
  {
    delay(300);
    if (!digitalRead(BUTTON)) {
      wifi.resetSetting();
      led.blinkMe(0); led.blinkMe(0);
      ESP.restart();
    }
  }
  eprom.epromWriteChar(200, '0');
  WatchDogCount = 0;
  ESP.deepSleep(0);

}

void loop() {
}

#include <FS.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <EEPROM.h>

volatile int WatchDogCount = 0;
int sensorValue = 0;
float battery_voltage;
int addr = 200;//eeprom address

ADC_MODE(ADC_VCC);


const char* host = "192.168.11.4";

Ticker Tick;
WiFiClient client;

int SensorPin = 3;//reed switch
int ledPin = 2;//LED pin

//watchdog function
void ISRWatchDog() {
  WatchDogCount++;
  EEPROM.write(addr, '1');
  EEPROM.commit();
  if (WatchDogCount > 300) {
    Serial.print("ISRWatchDog function executed...>>");
    ESP.reset();
  }
}

void blinkMe() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void setup() {

  pinMode(SensorPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  //wifi settings for initial hotspot
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  //wifiManager.resetSettings(); //this line only for reset stored password
  if (!wifiManager.autoConnect("rokkhi_V3B@", "rokkhi_V3B@")) {
    delay(3000);
    ESP.restart();
    delay(1000);
  }
  EEPROM.begin(512);
  Serial.begin(115200);
  //watchdoh attach
  Tick.attach(1, ISRWatchDog);
  //connected server wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" ");

  //data collection
  sensorValue = digitalRead(SensorPin);
  delay(5);
  Serial.print("sensor value: "); Serial.println(sensorValue);

  battery_voltage = ((float)ESP.getVcc()) / 1024;
  Serial.print("battery status: "); Serial.println(battery_voltage);

  char eeprom = char(EEPROM.read(addr));
  Serial.print("eeprom value:"); Serial.println(eeprom);

  String g = "";
  String f = "";
  String total = "";
  f += sensorValue;
  g += battery_voltage;
  total += f + "@" + g;

  //data send to host
  if (client.connect(host, 80)) {
    if (eeprom == '0') {
      String url = "/update?value=";
      url += total;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" +
                   "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
      delay(10);

      //acknowledgement from host
      while (client.available()) {
        String line = client.readStringUntil('\r');
        blinkMe(); blinkMe(); blinkMe();
      }
    }
    else {
      Serial.print("reset from watchdog");
    }
  }
  //eeprom rewrite 0
  EEPROM.write(addr, '0');
  EEPROM.commit();
  WatchDogCount = 0;
  Serial.println("Device in sleep mode");
  ESP.deepSleep(0);

}

void loop() {

}

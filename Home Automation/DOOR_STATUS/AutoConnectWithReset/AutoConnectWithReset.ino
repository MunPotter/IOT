#include <FS.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

HTTPClient http;
Ticker Tick;

const char* serverName = "http://api.rokkhi.com:8000/iot";
//const char* serverName = "http://home.api.rokkhi.com/api/v1/fingerprint-device/registerFingerPrint";
//const char* serverName3 = "http://home.api.rokkhi.com/api/v1/common/getIdToken";

String st;
//String auth;
String content;
int statusCode = 0;

volatile int WatchDogCount = 0;
int addr = 200;//eeprom address
int counter = 0;

int SensorPin = 3;//reed switch
int ledPin = 2;//LED pin 2

int sensor_status;
float battery_voltage;

ADC_MODE(ADC_VCC);

//void printMe(String str) {
//  Serial.println(str);
//}

void ISRWatchDog() {
  WatchDogCount++;
  EEPROM.write(addr, '1');
  EEPROM.commit();
  if (WatchDogCount > 300) {
//    Serial.print("ISRWatchDog function executed...>>");
    ESP.reset();
  }
}

void setup() {

  //  Serial.begin(9600);
  pinMode(SensorPin, INPUT_PULLUP);
  //  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  EEPROM.begin(512);
  Tick.attach(1, ISRWatchDog);

  //wifi settings
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
//    wifiManager.resetSettings();
  String wifimac = WiFi.macAddress();
  String defaults = "rokkhi_";
  String rokkhi = defaults + wifimac;

  char mmmm [20];
  rokkhi.toCharArray(mmmm, 30);

  if (!wifiManager.autoConnect(mmmm, mmmm)) {
    delay(3000);
    ESP.restart();
    delay(1000);
  }
  char eeprom = char(EEPROM.read(addr));
  if (eeprom == '0') {
    //  send to server
    SendToServer();
    delay(200);
  }
  EEPROM.write(addr, '0');
  EEPROM.commit();
  WatchDogCount = 0;
  //  printMe("going to deep sleep");
  ESP.deepSleep(0);

}

void blinkMe() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void loop() {

}



//
//
////--------------------------GET AUTH---------------------------------------------
//String httpGETRequest(const char* server) {
//  WiFiClient client2;
//
//  // Your IP address with path or Domain name with URL path
//  http.begin(client2, server);
//
//  // Send HTTP POST request
//  String uid = "community1@test.com";
//  String pid = "123456";
//  String au = "{\"email\":\"" + uid + "\",\"password\":\"" + pid + "\"}";
//  int httpResponseCode = http.POST(au);
//
//  String payload = "";
//  String payload2 = "";
//
//  if (httpResponseCode > 0) {
//    Serial.print("HTTP Response code for get auth: ");
//    Serial.println(httpResponseCode);
//    payload = http.getString();
//    DynamicJsonBuffer jsonBuffer(4096);
//    JsonObject& root = jsonBuffer.parseObject(http.getString());
//    const char* d = root["data"];
//    payload2 += d;
//  }
//  else {
//    Serial.print("Error code: ");
//    Serial.println(httpResponseCode);
//  }
//  // Free resources
//  http.end();
//  //  Serial.print(payload2);
//  return payload2;
//}
//
//
//---------------------------------------REGISTER DATA SEND--------------------------------------
String SendToServer() {
  String m = WiFi.macAddress();
  Serial.println(m);
  String w = WiFi.SSID();
  Serial.println(w);
  String ip = WiFi.localIP().toString();

  String door;
  sensor_status = analogRead(SensorPin);
  if (sensor_status == 0) {
    door = "close";
  }
  else {
    door = "open";
  }
  battery_voltage = ((float)ESP.getVcc()) / 1024;

  String f = "";
  String g = "";

  f += door;
  g += battery_voltage;

  //  auth = httpGETRequest(serverName3);
  //  Serial.println(auth);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    //prepare JSON data from
    //    String z = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    String z = "{\"doorStatus\":\"" + f + "\",\"batteryStatus\":\"" + g + "\",\"SSID\":\"" + w + "\",\"ipAddress\":\"" + ip + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    //    http.addHeader("authtoken", auth);

    //    Serial.print(z);
    int httpResponseCode = http.POST(z);
    delay(200);
    //    Serial.print("HTTP Response code for registration: ");
    //    Serial.println(httpResponseCode);
    if ((httpResponseCode == 201) || (httpResponseCode == 200)) {
      //      Serial.println("http: success");
      blinkMe(); blinkMe(); blinkMe();
      delay(1000);
    }
    else {
      //      Serial.println("http: failed");
      blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe();
      delay(1000);
    }
    http.end();
  }
  else {
    blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe();
  }
}

#include <FS.h>
#include <WiFiManager.h>
//#include "espnow.h"
//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <WiFiClient.h>
//#include <EEPROM.h>
//#include <ESP8266WebServer.h>
//#include <ArduinoJson.h>
//
//HTTPClient http;


String st;
String content;
int statusCode = 0;
int flag = 1;

//Your Domain name with URL path or IP address with path
const char* serverName = "http://home.api.rokkhi.com/api/v1/fingerprint-device/registerFingerPrint";

//auth url
const char* serverName3 = "http://home.api.rokkhi.com/api/v1/common/getIdToken";
String auth;


int DonePin = 12;//work done
int SensorPin = 0;//sensor value

int sensor_status;
float battery_voltage;
ADC_MODE(ADC_VCC);

void setup() {

//  Serial.begin(115200);
  pinMode(DonePin, OUTPUT);
  pinMode(SensorPin, INPUT_PULLUP);

  //digitalWrite(DonePin, HIGH);


  //wifi settings
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  //  wifiManager.resetSettings();
  if (!wifiManager.autoConnect("rokkhi", "")) {
    delay(3000);
    ESP.restart();
    delay(5000);
  }


  delay(200);
  //  send to server
  //need to add code for http request
  //  SendToServer();


}

void loop() {

  if (flag) {
    digitalWrite(DonePin, HIGH);
    for (int i = 0; i < 100; i++) {
      delay(100);
//      if (i > 70) {
//
//        digitalWrite(DonePin, LOW);
//        delay(50);
//        digitalWrite(DonePin, HIGH);
//      }
      if (i == 99) flag = 0;
    }
  }
  digitalWrite(DonePin, LOW);

}



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
//    const char* d = root["data"]; // "Sincere@april.biz"
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
////---------------------------------------REGISTER DATA SEND--------------------------------------
//String SendToServer() {
//
//  String m = WiFi.macAddress();
//  Serial.println(m);
//
//  sensor_status = digitalRead(SensorPin);
//  battery_voltage = ((float)ESP.getVcc()) / 1024;
//
//  String f = "";
//  String g = "";
//
//  f += sensor_status;
//
//
//  g += battery_voltage;
//
//  auth = httpGETRequest(serverName3);
//  //  Serial.println(auth);
//
//  if (WiFi.status() == WL_CONNECTED) {
//    WiFiClient client;
//
//    // Your Domain name with URL path or IP address with path
//    http.begin(client, serverName);
//    //prepare JSON data from
//    String z = "{\"sensorValue\":\"" + f + "\",\"batteryStatus\":\"" + g + "\",\"macAddressOfDevice\":\"" + m + "\"}";
//    // If you need an HTTP request with a content type: application/json, use the following:
//    http.addHeader("Content-Type", "application/json");
//    http.addHeader("authtoken", auth);
//
//    //    Serial.print(z);
//    int httpResponseCode = http.POST(z);
//    delay(200);
//    Serial.print("HTTP Response code for registration: ");
//    Serial.println(httpResponseCode);
//    if ((httpResponseCode == 201) || (httpResponseCode == 201)) {
//      Serial.println("http: success");
//    }
//    else {
//      Serial.println("http: failed");
//    }
//    http.end();
//  }
//  else {
//    Serial.println("WiFi Disconnected");
//  }
//
//}

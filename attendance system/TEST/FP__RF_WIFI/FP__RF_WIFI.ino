#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h>
#include <Wire.h>


#define RFID 1
#define FP 2

HTTPClient http;

String finalSoftValue;
String finalSoftValue2;

//const char* ssid = "Scitechbd";
//const char* password = "Rokkhi2020";
//
//
//String st;
//String content;
//int statusCode;
//
////Your Domain name with URL path or IP address with path
//const char* serverName = "http://home.api.rokkhi.com/api/v1/fingerprint-device/registerFingerPrint";
//
////Your Domain name with URL path or IP address with path
//const char* serverName2 = "http://home.api.rokkhi.com/api/v1/fingerprint-device/matchFingerPrintWithUser";
//
//
////auth url
//const char* serverName3 = "http://home.api.rokkhi.com/api/v1/common/getIdToken";
//String auth;
//
//
//
//unsigned long lastTime = 0;
//
//unsigned long timerDelay = 3000;



//---------------------------------Setup-------------------------------------
void setup()
{

  Serial.begin(9600);

  Wire.begin();
  //  WiFi.begin(ssid, password);
  //  Serial.println("");
  //  Serial.print("Connecting");
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //    Serial.print(".");
  //  }
  //  Serial.println("");
  //  Serial.print("Connected to WiFi network with IP Address: ");
  //  Serial.println(WiFi.localIP());
  //  Serial.println("");


  Serial.println("swap your card or put finger for Attendance");
  Serial.println("");
  delay(200);
}


//------------------------------------loop----------------------------------
void loop() {
  Wire.requestFrom(RFID, 8);

  while (Wire.available()) {
    delay(2);
    char SoftValue = (char)Wire.read();
    finalSoftValue += SoftValue;
    //    Serial.println(finalSoftValue);
  }
  delay(200);
  Wire.requestFrom(FP, 8);

  while (Wire.available()) {
    delay(2);
    char SoftValue2 = (char)Wire.read();
    finalSoftValue2 += SoftValue2;
    //    Serial.println(finalSoftValue);
  }

  for (int i = 0, j; finalSoftValue[i] != '\0'; ++i) {

    while (!(finalSoftValue[i] >= 'a' && finalSoftValue[i] <= 'z') && !(finalSoftValue[i] >= 'A' && finalSoftValue[i] <= 'Z') && !(finalSoftValue[i] >= '0' && finalSoftValue[i] <= '9') && !(finalSoftValue[i] == '\0')) {
      for (j = i; finalSoftValue[j] != '\0'; ++j) {
        finalSoftValue[j] = finalSoftValue[j + 1];
      }
      finalSoftValue[j] = '\0';
    }
  }
    for (int i = 0, j; finalSoftValue2[i] != '\0'; ++i) {

    while (!(finalSoftValue2[i] >= 'a' && finalSoftValue2[i] <= 'z') && !(finalSoftValue2[i] >= 'A' && finalSoftValue2[i] <= 'Z') && !(finalSoftValue2[i] >= '0' && finalSoftValue2[i] <= '9') && !(finalSoftValue2[i] == '\0')) {
      for (j = i; finalSoftValue2[j] != '\0'; ++j) {
        finalSoftValue2[j] = finalSoftValue2[j + 1];
      }
      finalSoftValue2[j] = '\0';
    }
  }
  Serial.println(finalSoftValue);
  Serial.println(finalSoftValue2);
  finalSoftValue = "";
  finalSoftValue2 = "";

}

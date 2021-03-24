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

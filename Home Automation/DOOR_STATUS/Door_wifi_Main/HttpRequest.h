#include <FS.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>


#include "Led.h"



class httpRequest {
  public:
    HTTPClient http;
    const char* serverName = "http://api.rokkhi.com:8000/iot";

    String st;
    String content;
    int statusCode = 0;

    String SendToServer(String datas) {

      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        http.begin(client, serverName);

        String z = datas;

        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(z);
        delay(200);

        if ((httpResponseCode == 201) || (httpResponseCode == 200)) {
          led.blinkMe(0);led.blinkMe(0);led.blinkMe(0);
          delay(300);
        }
        else {
          led.blinkMe(0); led.blinkMe(0); led.blinkMe(0);
          led.blinkMe(0); led.blinkMe(0);
          delay(300);
        }
        http.end();
      }
      else {
        led.blinkMe(0); led.blinkMe(0); led.blinkMe(0);
        led.blinkMe(0); led.blinkMe(0); led.blinkMe(0);
        led.blinkMe(0); led.blinkMe(0); led.blinkMe(0);
        led.blinkMe(0);
      }
    }
};

httpRequest httpRequest;

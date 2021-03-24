#include <FS.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* Regserver = "http://api.rokkhi.com:8000/registerFinger";
const char* Atdserver = "http://api.rokkhi.com:8000/matchFinger";

class httpRequest {
  public:
    HTTPClient http;

    String st;
    String content;
    int statusCode = 0;

    String SendToServer(String datas) {
      dis.DisplayConfig();
      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        http.begin(client, Regserver);

        String z = datas;
        Serial.print(z);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(z);
        delay(200);
        if ((httpResponseCode == 201) || (httpResponseCode == 200)) {
          dis.TextDisplay("");
          dis.TextDisplay("Sync: OK");
          Serial.println("Sync: OK");
          dis.FinishDisplay();

        }
        else {
          dis.TextDisplay("");
          dis.TextDisplay("Sync: Fail");
          Serial.println("Sync: Fail");
          dis.FinishDisplay();

        }
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
    }


    String SendToServer2(String datas) {

      dis.DisplayConfig();

      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        http.begin(client, Atdserver);

        String z = datas;

        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(z);
        delay(200);
        if ((httpResponseCode == 201) || (httpResponseCode == 200)) {
          dis.TextDisplay("");
          dis.TextDisplay("Sync: OK");
          Serial.println("Sync: OK");
          dis.FinishDisplay();
        }
        else {
          dis.TextDisplay("");
          dis.TextDisplay("Sync: Fail");
          Serial.println("Sync: Fail");
          dis.FinishDisplay();
        }
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
    }

};

httpRequest httpRequest;

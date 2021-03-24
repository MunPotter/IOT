
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

StaticJsonBuffer<200> jsonBuffer;

SoftwareSerial hostSerial (2, 16) ;
HTTPClient http;

String finalSoftValue;

String sensor;
String battery;
String t;
String id;
String power;
String cut;
String api = "rokkhi";

const char* ssid = "Scitechbd";
const char* password = "Rokkhi2020";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://api.rokkhi.com:8000/iot";


unsigned long lastTime = 0;

unsigned long timerDelay = 3000;

void setup() {
  Serial.begin(9600);
  hostSerial.begin(9600);

  DynamicJsonBuffer jsonBuffer;

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 3 seconds (timerDelay variable), it will take 3 seconds before publishing the first reading.");
}

void loop() {

  hostSerial.listen();
  delay(200);
  while (hostSerial.available() > 0) {
    delay(2);
    char SoftValue = (char)hostSerial.read();
    finalSoftValue += SoftValue;

  }
  if (finalSoftValue.length() > 0) {

    Serial.println("-----------------------------");
    sensor = split(finalSoftValue, ' ', 1);
    Serial.print("sensor value :");
    Serial.println(sensor);

    battery = split(finalSoftValue, ' ', 2);
    Serial.print("battery status :");
    Serial.println(battery);

    t = split(finalSoftValue, ' ', 3);
    Serial.print("time :");
    Serial.println(t);

    power = split(finalSoftValue, ' ', 5);
    Serial.print("power source :");
    Serial.println(power);


    cut = split(finalSoftValue, ' ', 6);
    Serial.print("fuel lid :");
    Serial.println(cut);

    id = split(finalSoftValue, ' ', 4);
    Serial.print("car number :");
    Serial.println(id);
    Serial.println("-----------------------------");
    Serial.println(" ");
    finalSoftValue = "";
  }

  Serial.print("current api :");
  Serial.println(api);
  delay(100);
  if (sensor == "0" || sensor == "1") {

    //gsm start working from here
    if (sensor != api) {

      api = sensor; //update api value
      Serial.print("updated api :");
      Serial.println(api);

      Serial.println("Sending data to Rokkhi api through gsm");
      Serial.println(" ");
      delay(500);


      //Check WiFi connection status
      if (WiFi.status() == WL_CONNECTED) {

        WiFiClient client;

        // Your Domain name with URL path or IP address with path
        http.begin(client, serverName);

        //prepare JSON data from
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& object = jsonBuffer.createObject();

        object.set("data", "@" + id + "@" + sensor + "@" + battery + "@" + t + "@" + cut + "@" + power + "@");
        object.printTo(Serial);
        Serial.println(" ");

        String sendtoserver;
        object.prettyPrintTo(sendtoserver);
        delay(200);


        // If you need an HTTP request with a content type: application/json, use the following:
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(sendtoserver);

        delay(200);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Free resources
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
      lastTime = millis();

    }
  }
  else {
    Serial.println("blank data receive");
  }

}


//string split function
String split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

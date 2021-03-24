#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial HZ1050(4, 14); // RX and TX

int i, j;
long value;

HTTPClient http;

int btn = 5;
int resetPin = 12;
int option = -1;

const char* ssid = "Scitechbd";
const char* password = "Rokkhi2020";

String st;
String content;
int statusCode;

//Your Domain name with URL path or IP address with path
const char* serverName = "http://home.api.rokkhi.com/api/v1/fingerprint-device/registerFingerPrint";

//Your Domain name with URL path or IP address with path
const char* serverName2 = "http://home.api.rokkhi.com/api/v1/fingerprint-device/matchFingerPrintWithUser";


//auth url
const char* serverName3 = "http://home.api.rokkhi.com/api/v1/common/getIdToken";
String auth;



unsigned long lastTime = 0;

unsigned long timerDelay = 3000;


//---------------------------------Setup-------------------------------------
void setup()
{
  pinMode(btn, INPUT_PULLUP);
  delay(1000);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  HZ1050.begin(9600);

  Serial.println("swap your card for Attendance");
  Serial.println("");
  delay(200);
}



//.-----------------------------------------loop-----------------------------------
void loop()                     // run over and over again
{

  value = NULL;

  if (HZ1050.available() > 0)
  {

    option = digitalRead(btn);
    delay(30);
    if (option == 0) {
      Serial.println("initiating user registration!");
      delay(500);
      Serial.println("");
      Serial.println("swap your card for Registration");
      delay(500);
      Serial.println("");
      delay(100);
      value = NULL;
      for (j = 0; j < 4; j++)
      {
        delay(100);
        i = HZ1050.read();
        value += ((long)i << (24 - (j * 8)));
      }


      Serial.print("UID:");
      Serial.println(value);

      SendToServer(value);
      Serial.println("Registration complete");
      delay(1000);
      Serial.println("");
    }
    else {

      delay(100);
      value = NULL;
      for (j = 0; j < 4; j++)
      {
        delay(100);
        i = HZ1050.read();
        value += ((long)i << (24 - (j * 8)));
      }

      Serial.print("UID:");
      Serial.println(value);

      SendToServer2(value);

      Serial.println("");
    }

    Serial.println("swap your card for Attendance");
    Serial.println("");
  }
}




//------------------------------------------ATTENDANCE DATA SEND----------------------------------------------------
long SendToServer2(long S) {

  String m = WiFi.macAddress();
  //  String ss = S;
  //  Serial.println(ss);
  String f = "";

  f += S;

  auth = httpGETRequest(serverName3);
  //  Serial.println(auth);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName2);
    //prepare JSON data from
    String z = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    http.addHeader("authtoken", auth);
    int httpResponseCode = http.POST(z);
    delay(200);
    Serial.print("HTTP Response code for attendance: ");
    Serial.println(httpResponseCode);
    if ((httpResponseCode == 201) || (httpResponseCode == 201)) {

      Serial.println("Sync: OK");

    }
    else {
      Serial.println("Status: Not Matched!");
      Serial.println("Sync: Fail");

    }

    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

}






//--------------------------GET AUTH---------------------------------------------
String httpGETRequest(const char* server) {
  WiFiClient client2;

  // Your IP address with path or Domain name with URL path
  http.begin(client2, server);

  // Send HTTP POST request
  String uid = "community1@test.com";
  String pid = "123456";
  String au = "{\"email\":\"" + uid + "\",\"password\":\"" + pid + "\"}";
  int httpResponseCode = http.POST(au);

  String payload = "";
  String payload2 = "";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code for get auth: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    DynamicJsonBuffer jsonBuffer(4096);
    JsonObject& root = jsonBuffer.parseObject(http.getString());
    const char* d = root["data"]; // "Sincere@april.biz"
    payload2 += d;
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload2;
}




//---------------------------------------REGISTER DATA SEND--------------------------------------
long SendToServer(long S) {

  String m = WiFi.macAddress();
  Serial.println(m);

  String f = "";

  f += S;

  auth = httpGETRequest(serverName3);


  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    //prepare JSON data from
    String z = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    http.addHeader("authtoken", auth);


    int httpResponseCode = http.POST(z);
    delay(200);
    Serial.print("HTTP Response code for registration: ");
    Serial.println(httpResponseCode);

    if ((httpResponseCode == 201) || (httpResponseCode == 201)) {

      Serial.println("http: success");
    }
    else {
      Serial.println("http: failed");
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

}

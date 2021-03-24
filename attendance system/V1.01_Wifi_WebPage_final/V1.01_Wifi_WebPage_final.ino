#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
int btn = 0;
SoftwareSerial mySerial(2, 16);
HTTPClient http;

int option = -1;


uint8_t id = 1;
int getFingerprintIDez();
const char *ssid = "test";
const char *password = "password";
String st;
String content;
int statusCode;
//Your Domain name with URL path or IP address with path
const char* serverName = "http://api.rokkhi.com:8000/iott";
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
ESP8266WebServer server(80);
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}





//...........................Setup.............................
void setup() {

  pinMode(btn, INPUT_PULLUP);
  delay(1000);
  Serial.begin(115200);

  //while(1){
  //  int a = digitalRead(btn);
  //  Serial.println(a);
  //  delay(100);
  //}

  Serial.println();
  Serial.print("Configuring access point…");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  createWebServer(1);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.begin(57600);
  if (finger.verifyPassword()) Serial.println("Found fingerprint sensor!");
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }
}






//.........................loop...............................
void loop()
{
  server.handleClient();

  String v = "";
  Serial.println("waiting for attendance.");
  Serial.println("no new user to register!");
  Serial.println(" ");
  v = attendance();
  Serial.println(v);
  SendToServer(v);

  deleteDB();
  delay(1000);
}


//.........................attendance function.................
String attendance() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK ) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK) Serial.println("Image taken");
    else {
      option = digitalRead(btn);
      delay(30);
      if (option == 1) {
        Serial.println("continiue attendance!");
      }
      if (option == 0) {
        Serial.println("initiating user registration!");
        finger.begin(57600);
        if (finger.verifyPassword()) Serial.println("Found fingerprint sensor!");
        String v = Register();
        Serial.println("sending registration data to server!");
        Serial.println(v);
        SendToServer(v);
      }
    }
  }
  p = finger.image2Tz(1);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) Serial.println("Stored!");
  delay(500);
  return uploadFingerpintTemplate();
}


String SendToServer(String S) {
  
  String m = WiFi.macAddress();
  
  char buffer[100];
  
  sprintf(buffer, "%s", WiFi.localIP().toString().c_str());

  String f = "";

  f += "@@" + m + "@@" + buffer + "@@" + S + "@@";
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    //prepare JSON data from
    String z = "{\"data\":\"" + f + "\"}";
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(z);
    delay(200);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}



//.........................Register function.................

String Register() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK)
    {
      Serial.println("Image taken");
    }
    else
    {
      Serial.print(".");
    }
  }
  p = finger.image2Tz(1);
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  p = finger.image2Tz(2);
  Serial.print("Creating model for #");  Serial.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return Register();
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return Register();
  } else {
    Serial.println("Unknown error");
    return Register();
  }
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    delay(500);
  }
  return uploadFingerpintTemplate();
}








//.........................uploadFingerpintTemplate function.................
String uploadFingerpintTemplate() {
  uint8_t p = finger.loadModel(1);
  if (p == FINGERPRINT_OK) {
    Serial.print("template "); Serial.print(id); Serial.println(" loaded");
  }
  p = finger.getModel();
  uint8_t templateBuffer[256];
  memset(templateBuffer, 0xff, 256);//zero out template buffer
  int index = 0;
  uint8_t arr[256];
  uint32_t starttime = millis();
  while ((index < 256) && ((millis() - starttime) < 1000))
  {
    if (mySerial.available())
    {
      templateBuffer[index] = mySerial.read();
      Serial.println("");
      index++;
    }
  }
  String str = "";
  Serial.println(" ");
  for (int m = 0; m < sizeof(templateBuffer); m++) {
    arr[m] = templateBuffer[m];
    str += arr[m];
  }
  return str;
}





//.......................Delete......................................
void deleteDB()
{
  finger.emptyDatabase();
  Serial.println("Now database is empty :)");
  delay(1000);
}
//......................CreateWebserver..........................
void createWebServer(int webtype)
{
  if ( webtype == 1 ) {
    server.on("/", []() {
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>WiFi configuration page for :  ";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();
        content = "{\"Success\":\"saved to eeprom....... reset to boot into new wifi\"}";
        statusCode = 200;
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.send(statusCode, "application/json", content);
    });
  } else if (webtype == 0) {
    server.on("/", []() {
      IPAddress ip = WiFi.localIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      server.send(200, "application/json", "{\"IP\":\"" + ipStr + "\"}");
    });
    server.on("/cleareeprom", []()
    {
      content = "<!DOCTYPE HTML>\r\n<html>";
      content += "<p>Clearing the EEPROM</p></html>";
      server.send(200, "text/html", content);
      Serial.println("clearing eeprom");
      for (int i = 0; i < 96; ++i) {
        EEPROM.write(i, 0);
      }
      EEPROM.commit();
    });
  }
}

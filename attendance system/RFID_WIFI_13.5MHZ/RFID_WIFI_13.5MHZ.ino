#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D0//9
#define SS_PIN D8//10


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

SoftwareSerial mySerial(2, 16);
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

void setup() {
  pinMode(btn, INPUT_PULLUP);
  delay(1000);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("swap your card to read");
  delay(200);
}


void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;

    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}

//.-----------------------------------------loop-----------------------------------
void loop()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    option = digitalRead(btn);
    delay(30);
    if (option == 0) {
      Serial.println("initiating user registration!");
      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
      }


      if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
      }

      char str[32] = "";
      array_to_string(mfrc522.uid.uidByte, 4, str);

      Serial.print("UID:"); //Print the output uid string
      Serial.println(str); //Print the output uid string
      String rf = "";
      rf += str;
      rf += "";
      SendToServer(rf);
      //      mfrc522.PICC_HaltA();
      Serial.println(rf);
      Serial.println("");
    }
    else{

      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
      }


      if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
      }

      char str[32] = "";
      array_to_string(mfrc522.uid.uidByte, 4, str);

      Serial.print("UID:"); //Print the output uid string
      Serial.println(str); //Print the output uid string
      String rf = "";
      rf += str;
      rf += "";
      SendToServer2(rf);
      //      mfrc522.PICC_HaltA();
      Serial.println(rf);
      Serial.println("");

    }
  }

}


//------------------------------------------ATTENDANCE DATA SEND----------------------------------------------------
String SendToServer2(String S) {

  String m = WiFi.macAddress();
  String ss = S;
  Serial.println(ss);
  String f = "";

  f += ss;

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
  //  Serial.print(payload2);
  return payload2;
}


//---------------------------------------REGISTER DATA SEND--------------------------------------
String SendToServer(String S) {

  String m = WiFi.macAddress();
  Serial.println(m);

  String ss = S;
  Serial.println(ss);
  String f = "";

  f += ss;

  auth = httpGETRequest(serverName3);
  //  Serial.println(auth);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    //prepare JSON data from
    String z = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    http.addHeader("authtoken", auth);

    //    Serial.print(z);
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

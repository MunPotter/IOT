#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



SoftwareSerial mySerial(2, 16);

#define RFID 1


HTTPClient http;

String finalSoftValue;
String finalSoftValue2;

String id;
String SensorCode;
String ActivityCode;

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


char sensorReadingsArr[5];
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


static const unsigned char PROGMEM image_data_Saraarray[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x03, 0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x0f, 0xf0, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x3f, 0xc0, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfd, 0xff, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x1f, 0x00, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x7f, 0xc0, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xc0, 0x7f, 0x80, 0x01, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xc0, 0x1f, 0xe0, 0x07, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xc0, 0x07, 0xf8, 0x3f, 0xe0, 0xfe, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1e, 0x00, 0x1f, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0x80, 0x3f, 0xc0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xc1, 0xff, 0x00, 0x7c, 0x01, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xfc, 0x00, 0x7e, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xe3, 0xff, 0xf0, 0x00, 0x7c, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0x80, 0xff, 0x80, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x03, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9d, 0xff, 0xf8, 0x7f, 0xff,
  0xff, 0xfc, 0x03, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x03, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xe7, 0x80, 0x03, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x0c, 0x0f, 0x9c, 0x01, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x78, 0x7c, 0x0c, 0x1f, 0x9c, 0x00, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x02, 0x00, 0xff, 0x03, 0xe0, 0x60, 0xfc, 0x08, 0x3f, 0x81, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0x00, 0x7c, 0x00, 0xf0, 0x41, 0xfc, 0x08, 0x1f, 0x99, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0x80, 0x38, 0x78, 0x70, 0x40, 0xfc, 0x0c, 0x0f, 0x9d, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0xc0, 0x1c, 0x30, 0x70, 0x60, 0x3c, 0x0e, 0x07, 0x81, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0xe0, 0x0e, 0x01, 0xe0, 0x78, 0x1c, 0x0f, 0x01, 0x81, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xff, 0x83, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xf3, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfe, 0x3f, 0xf7, 0xfc, 0xe5, 0xfd, 0x7b, 0x7f, 0xff, 0x7f, 0xf7, 0xdf, 0xff, 0xff, 0xff,
  0xff, 0xfd, 0xda, 0x94, 0xe5, 0xed, 0xad, 0x6b, 0x52, 0x9e, 0xb7, 0x74, 0xdc, 0x4a, 0x7f, 0xff,
  0xff, 0xfc, 0xda, 0x95, 0xd4, 0xec, 0xad, 0x4b, 0x5a, 0xdd, 0x99, 0x97, 0x5d, 0x5a, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

unsigned long lastTime = 0;

unsigned long timerDelay = 3000;



//---------------------------------Setup-------------------------------------
void setup()
{

  Serial.begin(9600);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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
  Serial.print("device MAC: ");
  String z = WiFi.macAddress();
  Serial.println(z);
  mySerial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  delay(200);
  display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);

  display.display();
  delay(3000);
  display.clearDisplay();

}


//------------------------------------loop----------------------------------
void loop() {


  display.setCursor(0, 7);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("PUT YOUR FINGER");
  display.println(" ");
  display.println("      OR");
  display.println(" ");
  display.println("SWAP YOUR ID CARD");
  display.display();
  display.startscrollright(0x00, 0x07);
  delay(1000);
  display.startscrollleft(0x00, 0x07);
  delay(1000);
  display.stopscroll();



  mySerial.listen();
  delay(50);
  Wire.requestFrom(RFID, 16);
  //  Wire.requestFrom(RFID, 16);
  delay(100);
  while (Wire.available() > 0) {
    char SoftValue = (char)Wire.read();
    finalSoftValue += SoftValue;
  }

  if (finalSoftValue != "0000000000000000") {
    if (finalSoftValue.length() > 0) {

      Serial.println("-----------------------------");
      ActivityCode = split(finalSoftValue, ' ', 1);
      Serial.print("Activity Code :");
      Serial.println(ActivityCode);

      SensorCode = split(finalSoftValue, ' ', 2);
      Serial.print("Sensor Code :");
      Serial.println(SensorCode);

      id = split(finalSoftValue, ' ', 3);
      Serial.print("Id :");
      Serial.println(id);
      Serial.println("-----------------------------");
      Serial.println(" ");
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 7);
      display.println("  DEVICE");
      display.println("");
      display.println("   RFID");
      display.display();
      finalSoftValue = "";


    }

  }


  delay(200);
  while (mySerial.available() > 0) {
    delay(2);
    //    Serial.println("inside while loop");
    char SoftValue2 = (char)mySerial.read();
    finalSoftValue2 += SoftValue2;

  }
  //  Serial.println("outside while loop");
  //  Serial.println(finalSoftValue2);
  if (finalSoftValue2.length() > 0) {


    Serial.println("-----------------------------");
    ActivityCode = split(finalSoftValue2, ' ', 1);
    Serial.print("Activity Code :");
    Serial.println(ActivityCode);

    SensorCode = split(finalSoftValue2, ' ', 2);
    Serial.print("Sensor Code :");
    Serial.println(SensorCode);

    id = split(finalSoftValue2, ' ', 3);
    Serial.print("Id :");
    Serial.println(id);
    Serial.println("-----------------------------");
    Serial.println(" ");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 7);
    display.println("  DEVICE");
    display.println("");
    display.println("    FP");
    display.display();
    finalSoftValue2 = "";
  }



  finalSoftValue = "";
  finalSoftValue2 = "";
  int a = ActivityCode.toInt();


  switch (a) {
    case 1:
      if (SensorCode == "811") {
        Serial.println("rfid attendance");
        char buf[8];
        id.toCharArray(buf, 8);
        Serial.println(buf[0]);
        if (buf[0] == '-') {
          Serial.println("error");
          Serial.println("again swap card");
        }
        else {
          SendToServer2(id);
          delay(1000);
        }
      } else if (SensorCode == "911") {
        Serial.println("finger print attendance");
        SendToServer2(id);
        delay(1000);
      }
      break;
    case 2:
      if (SensorCode == "811") {
        Serial.println("rfid registration");
        char buf[8];
        id.toCharArray(buf, 8);
        Serial.println(buf[0]);
        if (buf[0] == '-') {
          Serial.println("error");
          Serial.println("again swap card");
        }
        else {
          SendToServer(id);
          delay(1000);
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(0, 7);
          display.println("Wait...");
          display.display();
          display.clearDisplay();

        }
      }

      else if (SensorCode == "911") {
        Serial.println("finger print registration");
        SendToServer(id);
        delay(1000);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 7);
        display.println("Wait...");
        display.display();
        display.clearDisplay();

      }
      break;

    default:
      break;

  }
  ActivityCode = "";
  SensorCode = "";
  id = "";
  a = '\0';

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



//------------------------------------------ATTENDANCE DATA SEND----------------------------------------------------
String SendToServer2(String S) {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 7);


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
    //    display.print("Id:");
    //    display.println(f);
    int len = f.length();
    if ((httpResponseCode == 201) || (httpResponseCode == 200)) {

      if (len <= 4) {
        display.print("Id: ");
        display.println(f);
      } else {
        display.println("Matched");
      }
      display.println("");
      display.display();
      display.println("Success!");
      display.display();
      display.clearDisplay();
      Serial.println("Sync: OK");

    }
    else if ((httpResponseCode == 460) || (httpResponseCode == 400)) {
      display.println("");
      display.display();
      display.println("Not Match!");
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 7);
      display.println("");
      display.println("Try Again");
      display.display();
      display.clearDisplay();

      Serial.println("Status: Not Matched!");
      Serial.println("Sync: Fail");

    }
    else {
      display.println("");
      display.display();
      display.println("Server");
      display.println(" ");
      display.println("Error!");
      display.display();
      delay(500);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 7);
      display.println("");
      display.println("Try Again");
      display.display();
      display.clearDisplay();

      Serial.println("Status: Server Error!");
    }
    http.end();
    return "attendance";
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
String SendToServer(String S) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 7);
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

    if ((httpResponseCode == 201) || (httpResponseCode == 200)) {

      Serial.println("http: success");
      display.println("Enroll");
      display.println("");
      display.println("Success!!");
      display.display();
      delay(500);
      display.clearDisplay();
    }
    else if ((httpResponseCode == 460) || (httpResponseCode == 400)) {
      display.println("Enroll");
      display.println("");
      display.println("Failed!!");
      display.display();
      delay(500);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 7);
      display.println("");
      display.println("Try Again");
      display.display();
      display.clearDisplay();
      Serial.println("Reg:Failed");
    }
    else {
      display.println("Server");
      display.println(" ");
      display.println("Error!");
      display.display();
      delay(500);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 7);
      display.println("");
      display.println("Try Again");
      display.display();
      display.clearDisplay();
      Serial.println("Server Error!");
    }
    // Free resources
    http.end();
    return "registration";

  }
  else {
    Serial.println("WiFi Disconnected");
  }

}

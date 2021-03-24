#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 16);

HTTPClient http;

uint8_t id = 1;

int getFingerprintIDez();

const char* ssid = "Scitechbd";
const char* password = "Rokkhi2020";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://api.rokkhi.com:8000/iott";
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);



//...............................Setup.........................................

void setup()
{
  Serial.begin(115200);

  delay(1000);
  while (!Serial);
  delay(100);

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
  delay(500);


  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }

}


uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


//...................................loop...................................

void loop() {
  Serial.println(id);
  String v = getFingerprintEnroll();
  String f="";
  f+="@"+v+"@";
  Serial.print("fingrprint data: ");
  Serial.println(f);

    //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    //prepare JSON data from
    String x="{\"data\":\""+f+"\"}";

    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(x);

    delay(200);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
//  Wifi(v);
  deleteDB();
}


//.........................getFingerprintEnroll function.................


String getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK) Serial.println("Image taken");
    else Serial.print(".");
  }
  p = finger.image2Tz(1);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) Serial.println("Stored!");
  delay(500);
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
  //dump entire templateBuffer.  This prints out 16 lines                          of 16 bytes
  for (int count = 0; count < 16; count++)
  {
    for (int i = 0; i < 16; i++)
    {
      //Serial.print(templateBuffer[count * 16 + i], HEX);
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


//.......................Wifi..................................
//void Wifi(String d)
//{
//
//}

void deleteDB()
{
  finger.emptyDatabase();
  Serial.println("Now database is empty :)");
  delay(1000);
}

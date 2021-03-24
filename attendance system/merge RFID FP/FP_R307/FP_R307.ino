#include <ArduinoJson.h>
#include<SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>

int btn = 5;
int resetPin = 4;

SoftwareSerial mySerial(2, 3);
SoftwareSerial mySerial1(6, 7);

int option = -1;

int getFingerprintIDez();
String finalSoftValue2;


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


String SensorCode = "911";//rf id sensor code
String ActivityCode1 = "0";//attendance
String ActivityCode2 = "1";//


void(* resetTheBoard) (void) = 0;

//---------------------------------Setup-------------------------------------
void setup()
{
  pinMode(btn, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  delay(1000);
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial1.begin(9600);

  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }

  delay(5000);
}


//.-----------------------------------------loop-----------------------------------
void loop()                     // run over and over again
{




  Serial.println("Attandance Started");

  Serial.println("Attendance");
  Serial.println("");
  Serial.println("Put Finger");

  int i = getFingerprintID();


  Serial.println("Wait....");

  Serial.print("found id :");
  Serial.println(i);

  if (i == 0) {

    Serial.println("Wait....");
    delay(300);
    Serial.println("Not Found");

  }
  else {

    Serial.print("Id :");
    Serial.println(i);

    String v = " ";

    v += ActivityCode1;
    v += " ";
    v += SensorCode;
    v += " ";
    v += i;
    v += " ";

    int vLength = v.length();

    char finalSoftValue[vLength + 1];

    strcpy(finalSoftValue, v.c_str());

    mySerial1.write(finalSoftValue);
    Serial.print("passing data to RFID_FP_WIFI :");
    Serial.println(finalSoftValue);

    //    SendToServer2(i);
  }

  delay(2000);


}
//--------------------------------------------Registration-----------------------------------------------------------
int Registration() {

  Serial.println("NEW USER");
  Serial.println("");
  Serial.println("Put Finger");

  delay(10);


  int num = finger.templateCount;
  int id = num + 1;

  Serial.print("Enrolling ID #");
  Serial.println(id);
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }
  // OK success!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return Registration();
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return Registration();
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return Registration();
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return Registration();
    default:
      Serial.println("Unknown error");
      return Registration();
  }

  Serial.print("Wait....");

  delay(1000);
  Serial.println("Remove finger");

  Serial.println("Remove ");
  Serial.println("Finger");


  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");

  Serial.println("Put Same");
  Serial.println("Finger");
  Serial.println("Again");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }
  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return Registration();
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return Registration();
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return Registration();
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return Registration();
    default:
      Serial.println("Unknown error");
      return Registration();
  }
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");

    Serial.println("Finger");
    Serial.print("Match");


  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return Registration();
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");

    Serial.println("Finger");
    Serial.print("Not Match");


    return Registration();
  } else {
    Serial.println("Unknown error");
    return Registration();
  }
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return Registration();
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return Registration();
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return Registration();
  } else {
    Serial.println("Unknown error");
    return Registration();
  }
  Serial.println("sending registration data to server!");
  Serial.println(id);
  String v = " ";

  v += ActivityCode2;
  v += " ";
  v += SensorCode;
  v += " ";
  v += id;
  v += " ";

  int vLength = v.length();

  char finalSoftValue[vLength + 1];

  strcpy(finalSoftValue, v.c_str());

  mySerial1.write(finalSoftValue);
  Serial.print("passing data to RFID_FP_WIFI :");
  Serial.println(finalSoftValue);

  //  SendToServer(id);
  delay(2000);

  Serial.println("Wait...");

  resetTheBoard();
  return id;

}




////---------------------------------------REGISTER DATA SEND--------------------------------------
//int SendToServer(int S) {
////  display.clearDisplay();
////  display.setTextSize(2);
////  display.setTextColor(WHITE);
////  display.setCursor(0, 7);
//  String m = WiFi.macAddress();
//  Serial.println(m);
//
//  int ss = S;
//  Serial.println(ss);
//  String f = "";
//
//  f += ss;
//
//  auth = httpGETRequest(serverName3);
//  Serial.println(auth);
//
//  if (WiFi.status() == WL_CONNECTED) {
//    WiFiClient client;
//
//    // Your Domain name with URL path or IP address with path
//    http.begin(client, serverName);
//    //prepare JSON data from
//    String z = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
//    // If you need an HTTP request with a content type: application/json, use the following:
//    http.addHeader("Content-Type", "application/json");
//    http.addHeader("authtoken", auth);
//    int httpResponseCode = http.POST(z);
//    delay(200);
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);
//    display.print("Id:");
//    display.println(f);
//    if ((httpResponseCode == 201) || (httpResponseCode == 201)) {
//
//      display.println("Sync: OK");
//      display.display();
//
//      Serial.println("http: success");
//    }
//    else {
//      display.println("Sync: Fail");
//      display.display();
//
//      Serial.println("http: failed");
//    }
//    // Free resources
//    http.end();
//  }
//  else {
//    Serial.println("WiFi Disconnected");
//  }
//
//}






////------------------------------------------ATTENDANCE DATA SEND----------------------------------------------------
//int SendToServer2(int S) {
//
//  String m = WiFi.macAddress();
//  int ss = S;
//  Serial.println(ss);
//  String f = "";
//
//  f += ss;
//
//  auth = httpGETRequest(serverName3);
//  Serial.println(auth);
//
//  if (WiFi.status() == WL_CONNECTED) {
//    WiFiClient client;
//    // Your Domain name with URL path or IP address with path
//    http.begin(client, serverName2);
//    //prepare JSON data from
//    String z = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
//    // If you need an HTTP request with a content type: application/json, use the following:
//    http.addHeader("Content-Type", "application/json");
//    http.addHeader("authtoken", auth);
//    int httpResponseCode = http.POST(z);
//    delay(200);
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);
//    if ((httpResponseCode == 201) || (httpResponseCode == 201)) {
//      display.println("Sync: OK");
//      Serial.println("Sync: OK");
//      display.display();
//    }
//    else {
//      display.println("Sync: Fail");
//      Serial.println("Sync: Fail");
//      display.display();
//    }
//    //    display.println(httpResponseCode);
//    // Free resources
//    http.end();
//  }
//  else {
//    Serial.println("WiFi Disconnected");
//  }
//
//}




////--------------------------GET AUTH---------------------------------------------
//String httpGETRequest(const char* server) {
//  WiFiClient client2;
//
//  // Your IP address with path or Domain name with URL path
//  http.begin(client2, server);
//
//  // Send HTTP POST request
//  String uid = "community1@test.com";
//  String pid = "123456";
//  String au = "{\"email\":\"" + uid + "\",\"password\":\"" + pid + "\"}";
//  int httpResponseCode = http.POST(au);
//
//  String payload = "";
//  String payload2 = "";
//
//  if (httpResponseCode > 0) {
//    Serial.print("HTTP Response code for get: ");
//    Serial.println(httpResponseCode);
//    payload = http.getString();
//    DynamicJsonBuffer jsonBuffer(4096);
//    JsonObject& root = jsonBuffer.parseObject(http.getString());
//    const char* d = root["data"]; // "Sincere@april.biz"
//    payload2 += d;
//  }
//  else {
//    Serial.print("Error code: ");
//    Serial.println(httpResponseCode);
//  }
//  // Free resources
//  http.end();
//  Serial.print(payload2);
//  return payload2;
//}

//-------------------------------------Match Finger print----------------------------------------------------
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  //  // OK success!
  //  p = finger.image2Tz();
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

        Registration();
      }
    }
  }
  p = finger.image2Tz();

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    //    display.println("Found");
  } else if (p == FINGERPRINT_NOTFOUND) {
    //    display.clearDisplay();
    Serial.println("Did not find a match");
    //    display.println("Did not find a match");
    return 0;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  //   found a match!
  int prints = finger.fingerID;
  Serial.print("Found ID #"); Serial.print(prints);
  //  display.print("Found ID #"); display.print(prints);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return prints;
}

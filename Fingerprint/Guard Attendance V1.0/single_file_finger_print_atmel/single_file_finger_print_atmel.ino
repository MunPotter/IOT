#include "attendance.h"


//#include<SoftwareSerial.h>
//#include <Adafruit_Fingerprint.h>
//
//
//#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
//


//SoftwareSerial stmSerial(6, 7); // RX, TX ports
//SoftwareSerial mySerial(2, 3); // RX, TX ports


int getFingerprintIDez();

//String finalSoftValue;
//String sensor;



//#else
//#define mySerial Serial1
//#endif
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//



//---------------------------------Setup-------------------------------------
void setup()
{

  Serial.begin(9600);
  //  stmSerial.begin(9600);
  soft.softSetup();

  reg.fingerBegin();
  reg.fingerSetup();

  //  finger.begin(57600);
  //  if (finger.verifyPassword()) {
  //    Serial.println("Found fingerprint sensor!");
  //  } else {
  //    Serial.println("Did not find fingerprint sensor :(");
  //    while (1) {
  //      delay(1);
  //    }
  //  }
  //  finger.getTemplateCount();
  //  if (finger.templateCount == 0) {
  //    Serial.println("Sensor doesn't contain any fingerprint data.");
  //    Serial.println("please, enroll finger");
  //  }
  //  else {
  //    Serial.println("Waiting for valid finger...");
  //    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  //  }
  Serial.println("Attandance Started");

  delay(1000);
}


////string split function
//String split(String data, char separator, int index)
//{
//  int found = 0;
//  int strIndex[] = {0, -1};
//  int maxIndex = data.length() - 1;
//
//  for (int i = 0; i <= maxIndex && found <= index; i++) {
//    if (data.charAt(i) == separator || i == maxIndex) {
//      found++;
//      strIndex[0] = strIndex[1] + 1;
//      strIndex[1] = (i == maxIndex) ? i + 1 : i;
//    }
//  }
//  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
//}

//.-----------------------------------------loop-----------------------------------
void loop()                     // run over and over again
{

  Serial.println("");
  Serial.println("Attendance");
  Serial.println("");
  Serial.println("Put Finger");
  delay(100);
  uint8_t ids = atd.getFingerprintID();


  //  Serial.println("Wait....");
  Serial.print("found id :");
  Serial.println(ids);

  if (ids == 0) {

    //    Serial.println("Wait....");
    Serial.println("please try again ");

  }
  else {

    Serial.print("Id :");
    Serial.println(ids);

    //    SendToServer2(i);
  }
  delay(2000);
}

//
////--------------------------------------------Registration-----------------------------------------------------------
//int Registration() {
//
//  Serial.println("NEW USER");
//  Serial.println("");
//  Serial.println("Put Finger");
//  delay(10);
//
//
//  finger.getTemplateCount();
//  int id = finger.templateCount + 1;
//
//  Serial.print("Enrolling ID #");
//  Serial.println(id);
//  int p = -1;
//  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
//
//  while (p != FINGERPRINT_OK) {
//    p = finger.getImage();
//    switch (p) {
//      case FINGERPRINT_OK:
//        Serial.println("Image taken");
//        break;
//      case FINGERPRINT_NOFINGER:
//        Serial.println(".");
//        break;
//      case FINGERPRINT_PACKETRECIEVEERR:
//        Serial.println("Communication error");
//        break;
//      case FINGERPRINT_IMAGEFAIL:
//        Serial.println("Imaging error");
//        break;
//      default:
//        Serial.println("Unknown error");
//        break;
//    }
//  }
//  // OK success!
//  p = finger.image2Tz(1);
//  switch (p) {
//    case FINGERPRINT_OK:
//      Serial.println("Image converted");
//      break;
//    case FINGERPRINT_IMAGEMESS:
//      Serial.println("Image too messy");
//      return Registration();
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
//      return Registration();
//    case FINGERPRINT_FEATUREFAIL:
//      Serial.println("Could not find fingerprint features");
//      return Registration();
//    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println("Could not find fingerprint features");
//      return Registration();
//    default:
//      Serial.println("Unknown error");
//      return Registration();
//  }
//
//  Serial.print("Wait....");
//  delay(1000);
//  Serial.println("Remove finger");
//
//  Serial.println("Remove ");
//  Serial.println("Finger");
//
//
//  delay(1000);
//  p = 0;
//  while (p != FINGERPRINT_NOFINGER) {
//    p = finger.getImage();
//  }
//  Serial.print("ID "); Serial.println(id);
//  p = -1;
//  Serial.println("Place same finger again");
//
//
//  Serial.println("Put Same");
//  Serial.println("Finger");
//  Serial.println("Again");
//
//
//  while (p != FINGERPRINT_OK) {
//    p = finger.getImage();
//    switch (p) {
//      case FINGERPRINT_OK:
//        Serial.println("Image taken");
//        break;
//      case FINGERPRINT_NOFINGER:
//        Serial.print(".");
//        break;
//      case FINGERPRINT_PACKETRECIEVEERR:
//        Serial.println("Communication error");
//        break;
//      case FINGERPRINT_IMAGEFAIL:
//        Serial.println("Imaging error");
//        break;
//      default:
//        Serial.println("Unknown error");
//        break;
//    }
//  }
//  // OK success!
//  p = finger.image2Tz(2);
//  switch (p) {
//    case FINGERPRINT_OK:
//      Serial.println("Image converted");
//      break;
//    case FINGERPRINT_IMAGEMESS:
//      Serial.println("Image too messy");
//      return Registration();
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
//      return Registration();
//    case FINGERPRINT_FEATUREFAIL:
//      Serial.println("Could not find fingerprint features");
//      return Registration();
//    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println("Could not find fingerprint features");
//      return Registration();
//    default:
//      Serial.println("Unknown error");
//      return Registration();
//  }
//  // OK converted!
//  Serial.print("Creating model for #");  Serial.println(id);
//  p = finger.createModel();
//  if (p == FINGERPRINT_OK) {
//    Serial.println("Prints matched!");
//
//  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Serial.println("Communication error");
//    return Registration();
//  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
//    Serial.println("Fingerprints did not match");
//
//
//
//    return Registration();
//  } else {
//    Serial.println("Unknown error");
//    return Registration();
//  }
//  Serial.print("ID:"); Serial.println(id);
//  p = finger.storeModel(id);
//  if (p == FINGERPRINT_OK) {
//    Serial.println("Stored!");
//  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Serial.println("Communication error");
//    return Registration();
//  } else if (p == FINGERPRINT_BADLOCATION) {
//    Serial.println("Could not store in that location");
//    return Registration();
//  } else if (p == FINGERPRINT_FLASHERR) {
//    Serial.println("Error writing to flash");
//    return Registration();
//  } else {
//    Serial.println("Unknown error");
//    return Registration();
//  }
//  Serial.println("sending registration data to server!");
//  Serial.println(id);
//
//  //  SendToServer(id);
//
//  Serial.println("Wait...");
//
//  //  resetTheBoard();
//  return id;
//
//}




//
////-------------------------------------Match Finger print----------------------------------------------------
//uint8_t getFingerprintID() {
//  uint8_t p = finger.getImage();
//
//  //    p = finger.image2Tz();
//  while (p != FINGERPRINT_OK ) {
//    p = finger.getImage();
//    if (p == FINGERPRINT_OK) Serial.println("Image taken");
//    else {
//
//      //      stmSerial.listen();
//      //      delay(200);
//      //      while (stmSerial.available() > 0) {
//      //        delay(2);
//      //        char SoftValue = (char)stmSerial.read();
//      //        finalSoftValue += SoftValue;
//      //
//      //      }
//      //      if (finalSoftValue.length() > 0) {
//      //        sensor = split(finalSoftValue, ' ', 1);
//      //        finalSoftValue = "";
//      //      }
//      String sensor = soft.readSoftData();
//
//      if (sensor == "reject") {
//        finger.begin(57600);
//      }
//      if (sensor == "accepts")  {
//        Serial.println("initiating user registration!");
//        finger.begin(57600);
//        if (finger.verifyPassword()) Serial.println("Found fingerprint sensor!");
//        reg.Registration();
//
//      }
//    }
//  }
//  p = finger.image2Tz();
//
//  // OK converted!
//  p = finger.fingerSearch();
//  if (p == FINGERPRINT_OK) {
//    Serial.println("Found a print match!");
//
//  } else if (p == FINGERPRINT_NOTFOUND) {
//
//    Serial.println("Did not find a match");
//
//    return 0;
//  } else {
//    Serial.println("Unknown error");
//    return p;
//  }
//  //   found a match!
//  int prints = finger.fingerID;
//  Serial.print("Found ID #"); Serial.print(prints);
//  Serial.print(" with confidence of "); Serial.println(finger.confidence);
//
//  return prints;
//}

#include<SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <ArduinoJson.h>
#include "HttpRequest.h"


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

SoftwareSerial mySerial(2, 16); // RX, TX ports

#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);



class Reg {

  public:

    void fingerBegin() {
      finger.begin(57600);
    }

    void fingerSetup() {
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
        Serial.println("Sensor doesn't contain any fingerprint data.");
        Serial.println("please, enroll finger");
      }
      else {
        Serial.println("Waiting for valid finger...");
        Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
      }
    }

    int Registration() {
      Serial.println("");
      Serial.println("PUT FINGER");

      dis.DisplayConfig();
      dis.TextDisplay("NEW USER");
      dis.TextDisplay("");
      dis.TextDisplay("Put Finger");
      dis.FinishDisplay();

      delay(10);

      uint8_t idss = getDupplicateCheck();
      Serial.print("found id :");
      Serial.println(idss);



      if (idss != 0) {
        Serial.println("duplicate finger !!Already registered.");
        Serial.println("please, place another finger");
        dis.DisplayConfig();
        dis.TextDisplay("Duplicate");
        dis.TextDisplay("");
        dis.TextDisplay("Finger");
        dis.FinishDisplay();
        delay(1000);
        dis.DisplayConfig();
        dis.TextDisplay("Try");
        dis.TextDisplay("Another");
        dis.TextDisplay("Finger");
        dis.FinishDisplay();
        delay(1000);
        Registration();
      }
      else {

        finger.getTemplateCount();
        int id = finger.templateCount + 1;

        Serial.print("Enrolling ID #");
        Serial.println(id);

        int p = -1;
        Serial.println("Waiting for valid finger");
        Serial.println("");
        Serial.println("Put Finger");
        delay(10);


        while (p != FINGERPRINT_OK) {
          p = finger.getImage();
          switch (p) {
            case FINGERPRINT_OK:
              Serial.println("Image taken");
              break;
            case FINGERPRINT_NOFINGER:
              Serial.println(".");

              break;
            default:
              Serial.println("Unknown error");
              break;
          }
        }

        p = finger.image2Tz(1);
        switch (p) {
          case FINGERPRINT_OK:
            Serial.println("Image converted");
            break;
          case FINGERPRINT_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            return Registration();
          default:
            Serial.println("Unknown error");
            return Registration();
        }
        dis.DisplayConfig();
        dis.TextDisplay("Wait....");
        dis.FinishDisplay();
        Serial.println("Wait...................................");
        delay(500);
        Serial.println("Remove finger");

        Serial.println("Remove finger");
        dis.DisplayConfig();
        dis.TextDisplay("Remove ");
        dis.TextDisplay("Finger");
        dis.FinishDisplay();
        delay(1000);

        p = 0;
        while (p != FINGERPRINT_NOFINGER) {
          p = finger.getImage();
        }
        Serial.print("ID "); Serial.println(id);
        p = -1;
        Serial.println("Place same finger again");

        dis.DisplayConfig();
        dis.TextDisplay("Put Same");
        dis.TextDisplay("Finger");
        dis.TextDisplay("Again");
        dis.FinishDisplay();

        while (p != FINGERPRINT_OK) {
          p = finger.getImage();
          switch (p) {
            case FINGERPRINT_OK:
              Serial.println("Image taken");
              break;
            case FINGERPRINT_NOFINGER:
              Serial.print(".");
              break;
            default:
              Serial.println("Unknown error");
              break;
          }
        }

        p = finger.image2Tz(2);
        switch (p) {
          case FINGERPRINT_OK:
            Serial.println("Image converted");
            break;
          case FINGERPRINT_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            return Registration();
          default:
            Serial.println("Unknown error");
            return Registration();
        }

        Serial.print("Creating model for #");  Serial.println(id);
        p = finger.createModel();
        if (p == FINGERPRINT_OK) {
          Serial.println("Prints matched!");
        } else if (p == FINGERPRINT_ENROLLMISMATCH) {
          Serial.println("Fingerprints did not match");
          dis.DisplayConfig();
          dis.TextDisplay("Finger");
          dis.TextDisplay("Not Match");
          dis.FinishDisplay();
          delay(2000);
          return Registration();
        } else {
          Serial.println("Unknown error");
          return Registration();
        }
        Serial.print("ID:"); Serial.println(id);
        dis.DisplayConfig();
        dis.intDisplay(id);
        dis.FinishDisplay();
        p = finger.storeModel(id);
        if (p == FINGERPRINT_OK) {
          Serial.println("Stored!");

          Serial.println("");
        } else {
          Serial.println("Unknown error");
          return Registration();
        }
        Serial.println("Wait...................................");


        String m = WiFi.macAddress();
        String f = "";

        f += id;
        String RegData = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
        httpRequest.SendToServer(RegData);
        delay(2000);
        dis.DisplayConfig();
        dis.TextDisplay("Enroll");
        dis.TextDisplay("complete");
        dis.FinishDisplay();
        delay(1000);
        dis.DisplayConfig();
        dis.TextDisplay("Attendance");
        dis.TextDisplay("");
        dis.TextDisplay("Put Finger");
        dis.FinishDisplay();
        //        resetTheBoard();
        return id;
      }
    }

    uint8_t getDupplicateCheck() {
      uint8_t p = finger.getImage();
      while (p != FINGERPRINT_OK ) {
        p = finger.getImage();
        if (p == FINGERPRINT_OK) ;
        else {
          Serial.println(".");
        }
      }
      p = finger.image2Tz();
      p = finger.fingerSearch();

      if (p == FINGERPRINT_OK) {
        Serial.println(".");

      } else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println(".");
        return 0;
      } else {
        Serial.println("Unknown error");
        return p;
      }
      int prints = finger.fingerID;
      Serial.print(".");
      return prints;
    }
};

Reg reg;

#include<SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

SoftwareSerial mySerial(2, 3); // RX, TX ports

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
        soft.softSetup();
        soft.writeStringSoftData("sensor not working");
        fingerBegin();
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
      soft.softSetup();
      soft.writeStringSoftData("PUT FINGER");
      fingerBegin();
      uint8_t idss = getDupplicateCheck();
      Serial.print("found id :");
      Serial.println(idss);
      soft.softSetup();
      soft.writeIntSoftData(idss);
      fingerBegin();

      if (idss != 0) {
        Serial.println("duplicate finger !!Already registered.");
        Serial.println("please, place another finger");
        soft.softSetup();
        soft.writeStringSoftData("duplicate finger !!Already registered.");
        fingerBegin();
        Registration();
      }
      else {

        finger.getTemplateCount();
        int id = finger.templateCount + 1;

        Serial.print("Enrolling ID #");
        Serial.println(id);
        soft.softSetup();
        soft.writeIntSoftData(id);
        fingerBegin();
        int p = -1;
        Serial.println("Waiting for valid finger");
        Serial.println("");
        Serial.println("Put Finger");
        soft.softSetup();
        soft.writeStringSoftData("Put Finger");
        fingerBegin();
        delay(10);


        while (p != FINGERPRINT_OK) {
          p = finger.getImage();
          switch (p) {
            case FINGERPRINT_OK:
              Serial.println("Image taken");
              break;
            case FINGERPRINT_NOFINGER:
              Serial.println(".");
              soft.softSetup();
              soft.writeStringSoftData(".");
              fingerBegin();
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

        Serial.println("Wait...................................");
        delay(1000);
        Serial.println("Remove finger");
        soft.softSetup();
        soft.writeStringSoftData("Remove finger");
        fingerBegin();
        delay(1000);

        p = 0;
        while (p != FINGERPRINT_NOFINGER) {
          p = finger.getImage();
        }
        Serial.print("ID "); Serial.println(id);
        p = -1;
        Serial.println("Place same finger again");
        soft.softSetup();
        soft.writeStringSoftData("Place same finger again");
        fingerBegin();


        while (p != FINGERPRINT_OK) {
          p = finger.getImage();
          switch (p) {
            case FINGERPRINT_OK:
              Serial.println("Image taken");
              break;
            case FINGERPRINT_NOFINGER:
              Serial.print(".");
              soft.softSetup();
              soft.writeStringSoftData(".");
              fingerBegin();
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
          soft.softSetup();
          soft.writeStringSoftData("Prints matched!");
          fingerBegin();
        } else if (p == FINGERPRINT_ENROLLMISMATCH) {
          Serial.println("Fingerprints did not match");
          soft.softSetup();
          soft.writeStringSoftData("Fingerprints did not match");
          fingerBegin();
          return Registration();
        } else {
          Serial.println("Unknown error");
          return Registration();
        }
        Serial.print("ID:"); Serial.println(id);
        p = finger.storeModel(id);
        if (p == FINGERPRINT_OK) {
          Serial.println("Stored!");
          soft.softSetup();
          soft.writeStringSoftData("Stored!");
          fingerBegin();
          Serial.println("");
        } else {
          Serial.println("Unknown error");
          return Registration();
        }
        Serial.println("Wait...................................");
        soft.softSetup();
        soft.writeStringSoftData("Wait...................................");
        fingerBegin();
        delay(3000);
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
          soft.softSetup();
          soft.writeStringSoftData(".");
          fingerBegin();
        }
      }
      p = finger.image2Tz();
      p = finger.fingerSearch();

      if (p == FINGERPRINT_OK) {
        Serial.println(".");
        soft.softSetup();
        soft.writeStringSoftData(".");
        fingerBegin();
      } else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println(".");
        soft.softSetup();
        soft.writeStringSoftData(".");
        fingerBegin();
        return 0;
      } else {
        Serial.println("Unknown error");
        return p;
      }
      int prints = finger.fingerID;
      Serial.print(".");
      soft.softSetup();
      soft.writeStringSoftData(".");
      fingerBegin();
      return prints;
    }
};

Reg reg;

#include "registration.h"

int btn = 0;
//int resetPin = 12;
volatile int WatchDogCount = 0;

class MatchFinger {
  public:
    int option = -1;

    uint8_t getFingerprintID() {
      uint8_t p = finger.getImage();

      //    p = finger.image2Tz();
      while (p != FINGERPRINT_OK ) {
        WatchDogCount = 0;
        p = finger.getImage();
        if (p == FINGERPRINT_OK) Serial.println("Image taken");
        else {
          option = digitalRead(btn);
          delay(30);
          if (option == 1) {
            //            Serial.println("continiue attendance!");
            int mun;
          }
          if (option == 0) {

            Serial.println("initiating user registration!");
            dis.DisplayConfig();
            dis.TextDisplay("");
            dis.TextDisplay("   ENROLL");
            dis.FinishDisplay();
            delay(1000);
            finger.begin(57600);
            if (finger.verifyPassword()) Serial.println("Found fingerprint sensor!");

            reg.Registration();
          }
        }
      }
      p = finger.image2Tz();

      // OK converted!
      p = finger.fingerSearch();
      if (p == FINGERPRINT_OK) {
        Serial.println("Found a print match!");
      } else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println("Did not find a match");
        return 0;
      } else {
        Serial.println("Unknown error");
        return p;
      }
      //   found a match!
      int prints = finger.fingerID;
      Serial.print("Most Matched ID #"); Serial.print(prints);
      Serial.print(" with confidence of "); Serial.println(finger.confidence);
      return prints;
    }
};
MatchFinger atd;

#include "SoftwareSerialCommunication.h"
#include "registration.h"

class MatchFinger {
  public:

    uint8_t getFingerprintID() {
      uint8_t p = finger.getImage();

      //    p = finger.image2Tz();
      while (p != FINGERPRINT_OK ) {
        p = finger.getImage();
        if (p == FINGERPRINT_OK) Serial.println("Image taken");
      }
      p = finger.image2Tz();

      // OK converted!
      p = finger.fingerSearch();
      if (p == FINGERPRINT_OK) {
        Serial.println("Found a print match!");
        soft.softSetup();
        soft.writeStringSoftData("Found a print match!");
        reg.fingerBegin();

      } else if (p == FINGERPRINT_NOTFOUND) {

        Serial.println("Did not find a match");
        soft.softSetup();
        soft.writeStringSoftData("Did not find a match");
        reg.fingerBegin();

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

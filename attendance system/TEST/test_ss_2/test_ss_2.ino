
#include <SPI.h>
#include <Wire.h>
#include<SoftwareSerial.h>




#define RFID 1

String id = "11111";

void setup() {
  Serial.begin(9600);
  Wire.begin(RFID);

}


void requestEvent() {
  int rsize = id.length();
  // Setup byte variable in the correct size
  byte response[5];

  // Format answer as array
  for (byte i = 0; i < 5; i++) {
    response[i] = (byte)id.charAt(i);
  }
  
  Wire.write(response, sizeof(response));
}

void loop() {

  Wire.onRequest(requestEvent);
  delay(500);


}

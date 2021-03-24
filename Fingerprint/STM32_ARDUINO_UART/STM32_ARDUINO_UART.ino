#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 9); // RX, TX ports

void setup() {
  // set the data baud rate 
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. 
  }
  mySerial.begin(115200);
}

void loop() { 
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

#include <SoftwareSerial.h>
SoftwareSerial mySerial =  SoftwareSerial(6, 7);


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  int device = 240;

  char str[4];
  sprintf(str, "%d", device);

  Serial.println(str);
 
  delay(200);
  mySerial.write(str,4); //Write the serial data
  delay(1000);
}

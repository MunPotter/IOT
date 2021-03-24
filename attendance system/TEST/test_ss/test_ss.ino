
#include <SPI.h>
#include <Wire.h>
#include<SoftwareSerial.h>

String id = "22222";
SoftwareSerial hostSerial (6, 7) ;
void setup() {
  Serial.begin(9600);
  hostSerial.begin(9600);
}




void loop() {

  String softValue = " ";
  softValue += id;
  softValue += " ";

  int softValueLength = softValue.length();

  char finalSoftValue[softValueLength + 1];

  strcpy(finalSoftValue, softValue.c_str());
  delay(1000);
  hostSerial.write(finalSoftValue);

}

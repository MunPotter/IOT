#include <SoftwareSerial.h>

SoftwareSerial stmSerial(2, 16); //Software serial for gsm module  //RX TX

String finalSoftValue;
void setup() {
  // set the data baud rate
  Serial.begin(9600);
  stmSerial.begin(9600);
}
void loop()
{
  //  String softValue = " ";
  //  softValue += "new";
  //  softValue += " ";
  //
  //  int softValueLength = softValue.length();
  //  char finalSoftValue[softValueLength + 1];
  //  strcpy(finalSoftValue, softValue.c_str());
  //  Serial.println(finalSoftValue);
  const char *finalSoftValue = "1001";
  stmSerial.write(finalSoftValue);
}

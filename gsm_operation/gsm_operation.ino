#include <SoftwareSerial.h>
SoftwareSerial hostSerial =  SoftwareSerial(6, 7); //Software serial from host module   //RX TX

String finalSoftValue;

void setup() {
  Serial.begin(9600);
  hostSerial.begin(9600);
}

void loop() {
  hostSerial.listen();
  while (hostSerial.available() > 0) {
    delay(2);
    char SoftValue = (char)hostSerial.read();
    finalSoftValue += SoftValue;
  }

  if (finalSoftValue.length() > 0) {

    String sensorValue = getValue(finalSoftValue, ' ', 1);
    Serial.print("sensor Value :");
    Serial.println(sensorValue);
    String battery = getValue(finalSoftValue, ' ', 2);
    Serial.print("Battery Status ");
    Serial.println(battery);

    finalSoftValue = "";
  }
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

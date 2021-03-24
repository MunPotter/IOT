
#include<SoftwareSerial.h>
SoftwareSerial stmSerial(6, 7); // RX, TX ports
String finalSoftValue;
void setup() {
  stmSerial.begin(9600);
}

void loop() {
}

  String readSoftData() {
    stmSerial.listen();
    delay(200);
    while (stmSerial.available() > 0) {
      delay(2);
      char SoftValue = (char)stmSerial.read();
      finalSoftValue += SoftValue;

    }
    if (finalSoftValue.length() > 0) {
      softdata1 = split(finalSoftValue, ' ', 1);
      finalSoftValue = "";
    }
    return softdata1;
  }



//string split function
String split(String data, char separator, int index)
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

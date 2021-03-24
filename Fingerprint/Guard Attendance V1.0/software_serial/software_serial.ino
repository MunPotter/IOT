#include <SoftwareSerial.h>

SoftwareSerial stmSerial(6, 7); //Software serial for gsm module  //RX TX

String finalSoftValue;
String softdata1;
String sdata1;
String sdata2;

void setup() {
  Serial.begin(115200);
  stmSerial.begin(115200);
}
void loop()
{

  sdata1 = readSoftData();
  Serial.println(sdata1);
//
// delay()

  String softValue1 = "";
  softValue1 += "10011";
  softValue1 += " ";


  int softValueLength1 = softValue1.length();
  char finalSoftValue1[softValueLength1 + 1];
  strcpy(finalSoftValue1, softValue1.c_str());
//  if (sdata1 == "1001 ") {
    stmSerial.write(finalSoftValue1);
//  }


  sdata1 = " ";


  //  Serial.println(finalSoftValue2);


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

String readSoftData() {

  return softdata1;
}

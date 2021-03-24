#include "SoftwareSerialCommunication.h"

void setup()
{

  Serial.begin(115200);
  soft.softSetup();
}

void loop()
{
  String sensor = soft.readSoftData();
  Serial.println(sensor);
    delay(50);
//  if (sensor == "1001 ") {
//
//    soft.writeStringSoftData("10011");
//    delay(50);
//
//  }
  //  sensor="";
}

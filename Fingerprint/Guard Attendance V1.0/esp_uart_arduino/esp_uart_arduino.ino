#include "SoftwareSerialCommunication.h"
int counter = 0;
void setup()
{

  Serial.begin(115200);
  soft.softSetup();
}

void loop()
{
  String sensor = soft.readSoftData();
  Serial.println(sensor);
  //  delay(50);
  if (sensor == "esp" ) {
    if ( counter == 5) {
      delay(200);
      soft.writeStringSoftData("stm");


    }
    if (counter == 10) {
      delay(200);
      soft.writeStringSoftData("uno");

    }

    if (counter == 15) {
      delay(200);
      soft.writeStringSoftData("esp");
    }
  counter++;
  }
  delay(200);

}

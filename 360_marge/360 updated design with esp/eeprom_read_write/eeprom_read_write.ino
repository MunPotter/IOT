#include <EEPROM.h>

int addr = 0;

void setup()
{
  EEPROM.begin(512);  //Initialize EEPROM
  Serial.begin(115200);
  // write to EEPROM.
  EEPROM.write(addr, '1');
  EEPROM.commit();    //Store data to EEPROM
}

void loop()
{
  Serial.println("");
  Serial.print(char(EEPROM.read(addr)));
  delay(1000);
  EEPROM.write(addr, '0');
}

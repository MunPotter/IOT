#include <EEPROM.h>

class Eeprom {

  public:

    void epromWriteInt(int addr, int number)
    {
      EEPROM.write(addr, number >> 8);
      EEPROM.write(addr + 1, number & 0xFF);
    }
    
    int epromReadInt(int addr)
    {
      return (EEPROM.read(addr) << 8) + EEPROM.read(addr + 1);
    }

    void epromWriteChar(int addr, char c) {
      EEPROM.write(addr, c);
      EEPROM.commit();
    }

    char epromReadChar(int addr) {
      char ch = char(EEPROM.read(addr));
      return ch;
    }

    void epromWriteString(int addr, const String &strToWrite)
    {
      byte len = strToWrite.length();
      EEPROM.write(addr, len);
      for (int i = 0; i < len; i++)
      {
        EEPROM.write(addr + 1 + i, strToWrite[i]);
      }
    }

    String epromReadString(int addr)
    {
      int newStrLen = EEPROM.read(addr);
      char data[newStrLen + 1];
      for (int i = 0; i < newStrLen; i++)
      {
        data[i] = EEPROM.read(addr + 1 + i);
      }
      data[newStrLen] = '\0';
      return String(data);
    }
};
Eeprom eprom;

#include<SoftwareSerial.h>
SoftwareSerial stmSerial(6, 7); // RX, TX ports

class softSerial {
  public:

    String finalSoftValue;
    String softdata1;

    void softSetup() {
      stmSerial.begin(115200);
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

    void writeIntSoftData(int valu) {
      String sendsoftValue = " ";
      sendsoftValue += "@";
      sendsoftValue += valu;
      sendsoftValue += " ";


      int sendsoftValueLength = sendsoftValue.length();
      char sendfinalSoftValue[sendsoftValueLength + 1];
      strcpy(sendfinalSoftValue, sendsoftValue.c_str());
      //      Serial.println(sendfinalSoftValue);
      stmSerial.write(sendfinalSoftValue);

    }
    void writeStringSoftData(String str) {
      String sendsoftValue = "";
      sendsoftValue += "@";
      sendsoftValue += str;
      sendsoftValue += "";


      int sendsoftValueLength = sendsoftValue.length();
      char sendfinalSoftValue[sendsoftValueLength + 1];
      strcpy(sendfinalSoftValue, sendsoftValue.c_str());
      //      Serial.println(sendfinalSoftValue);
      stmSerial.write(sendfinalSoftValue);

    }

};

softSerial soft;

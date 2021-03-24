#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "HttpSettings.h"


SoftwareSerial gsmSerial(3, 5); // gsm software serial  //RX TX

class GSMConfig {

  public:
    StaticJsonBuffer<200> jsonBuffer;


    void gsmSetup() {
      gsmSerial.begin(9600);
      DynamicJsonBuffer jsonBuffer;
    }

    void gsmSend( String vall) {
      gsmSerial.listen();
      connectGPRS();
      sendGPRS(vall);
    }

    void sendGPRS(String HttpData) {
      //http request begin
      gsmSerial.println("AT+HTTPINIT");
      delay(200);
      ShowSerialData();

      gsmSerial.println("AT+HTTPPARA=\"CID\",1");
      delay(200);
      ShowSerialData();

      //prepare JSON data from
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& object = jsonBuffer.createObject();
      object.set("data", HttpData);
      object.printTo(Serial);
      Serial.println(" ");

      String sendtoserver;
      object.prettyPrintTo(sendtoserver);
      delay(200);
      String URL =hs.API();
      gsmSerial.println("AT+HTTPPARA=\"URL\",\"" + URL + "\""); //Server address
      delay(200);
      ShowSerialData();

      gsmSerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
      delay(2000);
      ShowSerialData();

      gsmSerial.println("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
      Serial.println(sendtoserver);
      delay(200);
      ShowSerialData();

      gsmSerial.println(sendtoserver);
      delay(200);
      ShowSerialData();

      gsmSerial.println("AT+HTTPACTION=1");
      delay(200);
      ShowSerialData();

      gsmSerial.println("AT+HTTPREAD");
      delay(200);
      ShowSerialData();

      gsmSerial.println("AT+HTTPTERM");
      delay(1000);
      ShowSerialData();

    }

    //gprs function
    void connectGPRS() {
      if (gsmSerial.available())
        Serial.write(gsmSerial.read());

      gsmSerial.println("AT");
      delay(200);

      gsmSerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
      delay(200);
      ShowSerialData();

      String apnName =hs.APN();
      String apnSettings = "AT+SAPBR=3,1,\"APN\",\"" + apnName + "\"";
      gsmSerial.println(apnSettings);//APN
      delay(200);
      ShowSerialData();

      gsmSerial.println("AT+SAPBR=1,1");
      delay(200);
      ShowSerialData();

      gsmSerial.println("AT+SAPBR=2,1");
      delay(200);
      ShowSerialData();
    }


    //printing gsm serial data
    void ShowSerialData()
    {
      while (gsmSerial.available() != 0)
        Serial.write(gsmSerial.read());
      delay(200);
    }


};

GSMConfig gsmc;

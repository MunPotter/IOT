#include <SoftwareSerial.h>
#include <ArduinoJson.h>


SoftwareSerial myserial(3, 5); // RX: 10, TX:11
StaticJsonBuffer<200> jsonBuffer;
SoftwareSerial mySerial =  SoftwareSerial(6, 7);

char mystr[4];
void setup()
{
  Serial.begin(9600);
  myserial.begin(9600);
  mySerial.begin(9600);
  
  DynamicJsonBuffer jsonBuffer;
  delay(1000);
}

void loop() {
  mySerial.listen();
  mySerial.readBytes(mystr, 4); //Read the serial data and store in var
  Serial.println(mystr);//Print data on Serial Monitor

  delay(1000);
  

  
//  myserial.listen();
//  connectGPRS();
//  myserial.println("AT+HTTPINIT");
//  delay(1000);
//  ShowSerialData();
//
//  myserial.println("AT+HTTPPARA=\"CID\",1");
//  delay(1000);
//  ShowSerialData();
//
//  StaticJsonBuffer<200> jsonBuffer;
//  JsonObject& object = jsonBuffer.createObject();
//
//  object.set("deviceID", mystr);
//
//
//  object.printTo(Serial);
//  Serial.println(" ");
//
//  String sendtoserver;
//  object.prettyPrintTo(sendtoserver);
//  delay(1000);
//
//  myserial.println("AT+HTTPPARA=\"URL\",\"http://api.rokkhi.com:8000/iot\""); //Server address
//  delay(1000);
//  ShowSerialData();
//
//  myserial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
//  delay(4000);
//  ShowSerialData();
//
//  myserial.println("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
//  Serial.println(sendtoserver);
//  delay(1000);
//  ShowSerialData();
//
//  myserial.println(sendtoserver);
//  delay(1000);
//  ShowSerialData;
//
//  myserial.println("AT+HTTPACTION=1");
//  delay(1000);
//  ShowSerialData();
//
//  myserial.println("AT+HTTPREAD");
//  delay(1000);
//  ShowSerialData();
//
//  myserial.println("AT+HTTPTERM");
//  delay(10000);
//  ShowSerialData;

}




void connectGPRS() {
  if (myserial.available())
    Serial.write(myserial.read());

  myserial.println("AT");
  delay(1000);

  myserial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(1000);
  ShowSerialData();

  myserial.println("AT+SAPBR=3,1,\"APN\",\"gpinternet\"");//APN
  delay(1000);
  ShowSerialData();

  myserial.println("AT+SAPBR=1,1");
  delay(1000);
  ShowSerialData();

  myserial.println("AT+SAPBR=2,1");
  delay(1000);
  ShowSerialData();
}

void connectHTTP() {

}

void ShowSerialData()
{
  while (myserial.available() != 0)
    Serial.write(myserial.read());
  delay(1000);

}

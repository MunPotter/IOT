#include <SoftwareSerial.h>
#include <ArduinoJson.h> //version 5.10 needed

#define red 9 //red light pin 
#define green 10 // green light pin

SoftwareSerial hostSerial(4, 2); // RX, TX ports
SoftwareSerial gsmSerial(5, 3); // RX, TX ports

int incomingByte;
String finalSoftValue;
String sensor;
String battery;
String mac;
String power;
String cut;


void setup()
{
  Serial.begin(115200);
  hostSerial.begin(9600);
  gsmSerial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  DynamicJsonBuffer jsonBuffer;
}


void loop() {
  hostSerial.listen();

  while (hostSerial.available() > 0) {
    delay(2);
    char SoftValue = (char)hostSerial.read();
    finalSoftValue += SoftValue;
  }
  Serial.println(finalSoftValue);
  delay(100);
  if (finalSoftValue.length() > 0) {

    Serial.println("-----------------------------");
    sensor = split(finalSoftValue, '@', 0);
    Serial.print("sensor value :");
    Serial.println(sensor);

    battery = split(finalSoftValue, '@', 1);
    Serial.print("battery status :");
    Serial.println(battery);

    power = split(finalSoftValue, '@', 2);
    Serial.print("power source :");
    Serial.println(power);

    cut = split(finalSoftValue, '@', 3);
    Serial.print("fuel lid :");
    Serial.println(cut);

    mac = split(finalSoftValue, '@', 4);
    Serial.print("mac address :");
    Serial.println(mac);
    Serial.println("-----------------------------");
    finalSoftValue = "";

    Serial.println("Sending data to Rokkhi api through gsm");
    Serial.println(" ");
    gsmSerial.listen();

    //gprs connection begin
    connectGPRS();

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
    object.set("data", "@" + sensor + "@" + battery + "@" + cut + "@" + power + "@" + mac + "@");

    object.printTo(Serial);
    Serial.println(" ");

    String sendtoserver;
    object.prettyPrintTo(sendtoserver);
    delay(200);

    gsmSerial.println("AT+HTTPPARA=\"URL\",\"http://api.rokkhi.com:8000/iot\""); //Server address
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
    ShowSerialData;

    gsmSerial.println("AT+HTTPACTION=1");
    delay(200);
    ShowSerialData();

    gsmSerial.println("AT+HTTPREAD");
    delay(200);
    ShowSerialData();

    gsmSerial.println("AT+HTTPTERM");
    delay(1000);
    ShowSerialData;
  }
  delay(100);
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

  gsmSerial.println("AT+SAPBR=3,1,\"APN\",\"gpinternet\"");//APN
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

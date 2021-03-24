
#include <SPI.h>
#include <Wire.h>
#include<SoftwareSerial.h>

SoftwareSerial HZ1050(2, 3); // RX and TX
#define RFID 1

int i, j;
long value;

int btn = 5;
//int resetPin = 12;
int option = -1;

//String st;
//String content;
//int statusCode;



String def = "00000000000000000";
String SensorCode = "811";//rf id sensor code
String ActivityCode1 = "1";//attendance
String ActivityCode2 = "2";//registration

//---------------------------------Setup-------------------------------------
void setup()
{
  pinMode(btn, INPUT_PULLUP);
  delay(1000);
  Serial.begin(9600);


  HZ1050.begin(9600);
  Wire.begin(RFID);
  Serial.println("swap your card for Attendance");
  Serial.println("");
  delay(200);
}




void requestEvent() {


  value = NULL;
  if (HZ1050.available() > 0)
  {
    option = digitalRead(btn);
    delay(30);
    if (option == 0) {
      Serial.println("initiating user registration!");
      delay(500);
      Serial.println("");
      Serial.println("swap your card for Registration");
      delay(500);
      Serial.println("");
      delay(100);
      value = NULL;
      for (j = 0; j < 4; j++)
      {
        delay(100);
        i = HZ1050.read();

        value += ((long)i << (24 - (j * 8)));
      }
      delay(50);
      String v = " ";

      v += ActivityCode2;
      v += " ";
      v += SensorCode;
      v += " ";
      v += value;
      v += " ";

        Serial.print("UID:");
        Serial.println(v);
        int rsize = v.length();
        // Setup byte variable in the correct size
        byte response[rsize];
        // Format answer as array
        for (byte m = 0; m < rsize; m++) {
          response[m] = (byte)v.charAt(m);
        }
        Wire.write(response, sizeof(response));
      
        Serial.println("");
        v = "";
      

    }
    else {
      delay(100);
      value = NULL;
      for (j = 0; j < 4; j++)
      {
        delay(100);
        i = HZ1050.read();

        value += ((long)i << (24 - (j * 8)));
      }

      String v = " ";

      v += ActivityCode1;
      v += " ";
      v += SensorCode;
      v += " ";
      v += value;
      v += " ";

      Serial.print("UID:");
      Serial.println(v);
      int rsize = v.length();
      // Setup byte variable in the correct size
      byte response[rsize];
      // Format answer as array
      for (byte m = 0; m < rsize; m++) {
        response[m] = (byte)v.charAt(m);
      }
      Wire.write(response, sizeof(response));
      Serial.println("");
    }
  }
  else {
    int rsize = def.length();
    // Setup byte variable in the correct size
    byte response[rsize];
    // Format answer as array
    for (byte m = 0; m < rsize; m++) {
      response[m] = (byte)def.charAt(m);
    }
    Wire.write(response, sizeof(response));
    Serial.println("");
  }

}
//.-----------------------------------------loop-----------------------------------
void loop()
{
  Wire.onRequest(requestEvent);
  delay(2000);

}










//
//#include <SPI.h>
//#include <Wire.h>
//#include<SoftwareSerial.h>
//
//SoftwareSerial HZ1050(2, 3); // RX and TX
//#define RFID 1
//
//int i, j;
//long value;
//
//int btn = 5;
//int resetPin = 12;
//int option = -1;
//
//String st;
//String content;
//int statusCode;
//
//String def = "blank";
//
//
////---------------------------------Setup-------------------------------------
//void setup()
//{
//  pinMode(btn, INPUT_PULLUP);
//  delay(1000);
//  Serial.begin(9600);
//
//
//  HZ1050.begin(9600);
//  Wire.begin(RFID);
//  Serial.println("swap your card for Attendance");
//  Serial.println("");
//  delay(200);
//}
//
//
//void requestEvent() {
//
//
//  value = NULL;
//  if (HZ1050.available() > 0)
//  {
//    delay(100);
//    value = NULL;
//    for (j = 0; j < 4; j++)
//    {
//      delay(100);
//      i = HZ1050.read();
//      value += ((long)i << (24 - (j * 8)));
//    }
//
//    String v = "";
//    v += value;
//
//    Serial.print("UID:");
//    Serial.println(v);
//    int rsize = v.length();
//    // Setup byte variable in the correct size
//    byte response[rsize];
//    // Format answer as array
//    for (byte m = 0; m < rsize; m++) {
//      response[m] = (byte)v.charAt(m);
//    }
//    Wire.write(response, sizeof(response));
//    Serial.println("");
//  }
//  else {
//    int rsize = def.length();
//    // Setup byte variable in the correct size
//    byte response[rsize];
//    // Format answer as array
//    for (byte m = 0; m < rsize; m++) {
//      response[m] = (byte)def.charAt(m);
//    }
//    Wire.write(response, sizeof(response));
//    Serial.println("");
//  }
//
//}
////.-----------------------------------------loop-----------------------------------
//void loop()
//{
//  Wire.onRequest(requestEvent);
//  delay(2000);
//
//}

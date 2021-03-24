#include "attendance.h"


int getFingerprintIDez();

void setup()
{

  Serial.begin(9600);
  soft.softSetup();

  reg.fingerBegin();
  reg.fingerSetup();

  Serial.println("Attandance Started");
  delay(1000);
}


void loop()
{

  Serial.println("");
  Serial.println("Attendance");
  Serial.println("...........");
  Serial.println("Put Finger");
  soft.softSetup();
  soft.writeStringSoftData("Put Finger");
  reg.fingerBegin();
  delay(100);

  soft.softSetup();
  String sensor = soft.readSoftData();

  if (sensor == "atd") {
    uint8_t ids = atd.getFingerprintID();
    if (ids == 0) {
      Serial.println("please try again ");
      soft.softSetup();
      soft.writeStringSoftData("please try again");
      reg.fingerBegin();
    }
    else {
      Serial.print("found id :");
      Serial.println(ids);
      soft.writeIntSoftData(ids);
      reg.fingerBegin();
      Serial.println("attendance complete!!");
      Serial.println(" ");
    }
  }
  else if ((sensor == "reg")) {

    soft.softSetup();
    String sensor2 = soft.readSoftData();

    if (sensor2 == "new") {
      Serial.println("");
      Serial.println("initiating new user registration!");
      Serial.println("..............................");
      reg.fingerBegin();
      finger.emptyDatabase();
      Serial.println("NEW USER");
      Serial.println("");
      soft.softSetup();
      soft.writeStringSoftData("initiating new user registration!");
      reg.fingerBegin();
      if (finger.verifyPassword()) Serial.println("");
      reg.Registration();
    }
    else {
      reg.fingerBegin();
    }
    if (sensor2 == "existing")  {
      Serial.println("");
      Serial.println("initiating existing user registration!");
      soft.softSetup();
      soft.writeStringSoftData("initiating existing user registration!");
      Serial.println("..............................");
      reg.fingerBegin();
      if (finger.verifyPassword()) Serial.println("");
      reg.Registration();
    }
    else {
      reg.fingerBegin();
    }
  }
  delay(2000);
}

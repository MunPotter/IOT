
#include <ArduinoJson.h>
#include <Ticker.h>

#include "WifiConfig.h"
#include "attendance.h"

Ticker Tick;

int counter = 0;
int getFingerprintIDez();

int door = 15;



//void(* resetTheBoard) (void) = 0;

void ISRWatchDog() {
  WatchDogCount++;
  if (WatchDogCount > 300000) {
    ESP.reset();
  }
}

void setup()
{
  WatchDogCount = 0;
  pinMode(btn, INPUT_PULLUP);
  pinMode(door, OUTPUT);
  digitalWrite(door, LOW);
  //  pinMode(resetPin, INPUT_PULLUP);
  Serial.begin(9600);

  Tick.attach(1, ISRWatchDog);


  reg.fingerBegin();
  reg.fingerSetup();
  dis.DisplaySetup();

  dis.RokkhiBitMap();
  delay(3000);
  wifi.wifiSetup();

  dis.DisplayConfig();
  dis.TextDisplay("");
  dis.TextDisplay("  WELCOME");
  dis.FinishDisplay();
  delay(2000);
  Serial.println("Attendance Started");
}


void loop()
{

  Serial.println("");
  Serial.println("Attendance");
  Serial.println("...........");
  Serial.println("Put Finger");
  delay(200);
  dis.DisplayConfig();
  dis.TextDisplay("Attendance");
  dis.TextDisplay("");
  dis.TextDisplay("Put Finger");
  dis.FinishDisplay();

  int ids = atd.getFingerprintID();

  dis.DisplayConfig();
  dis.TextDisplay("");
  dis.TextDisplay("Wait....");
  dis.FinishDisplay();
  dis.ClearDisplay();
  delay(300);

  if (ids == 0) {
    Serial.println("please try again ");
    dis.DisplayConfig();
    dis.TextDisplay("");
    dis.TextDisplay("Wait....");
    dis.FinishDisplay();
    delay(300);
    dis.DisplayConfig();
    dis.TextDisplay("");
    dis.TextDisplay("Not Found");
    dis.FinishDisplay();
  }
  else {
    Serial.print("found id :");
    Serial.println(ids);

    dis.DisplayConfig();
    dis.TextDisplay("");
    dis.intDisplay(ids);
    dis.FinishDisplay();
    digitalWrite(door, HIGH);
    delay(3000);
    digitalWrite(door, LOW);
    String m = WiFi.macAddress();
    String f = "";
    f += ids;
    String AtdData = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    httpRequest.SendToServer2(AtdData);
    Serial.println("attendance complete!!");
    Serial.println(" ");
    dis.FinishDisplay();
  }
  delay(2000);
  WatchDogCount = 0;
}

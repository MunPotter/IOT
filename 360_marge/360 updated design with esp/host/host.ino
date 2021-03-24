#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>

#define CarBatteryIN 3 //car battery
#define MainBatteryIN 4 //main battery
#define FuelLid 5 //fuel lid

IPAddress ip(192, 168, 11, 4);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server;
SoftwareSerial hostSerial(2, 16); //Software serial for gsm module  //RX TX
Ticker Tick;

String sensor_value = "";
String power;
String cut;
int carPower = 0;
int mainPower = 0;
int fuelLid = 0;
volatile int WatchDogCount = 0;

//watchdog function
void ISRWatchDog() {
  WatchDogCount++;
  if (WatchDogCount > 300) {
    Serial.print("ISRWatchDog function executed...>>");
    ESP.reset();
  }
}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  //  hotspot settings
  String m = WiFi.macAddress();
  String ssid_client = "";
  ssid_client = "rokkhi_V3A@" + m;
  String ssid_ap = ssid_client;
  String password_ap = ssid_client;
  WiFi.softAP(ssid_ap, password_ap);

  Serial.begin(115200);
  hostSerial.begin(9600);
  //watchdog attach
  Tick.attach(1, ISRWatchDog);

  pinMode(CarBatteryIN, INPUT);
  pinMode(MainBatteryIN, INPUT);
  pinMode(FuelLid, INPUT);
  Serial.println();

  // Configure the server's routes
  server.on("/", handleIndex);
  server.on("/update", handleUpdate);
  server.begin();
}

void loop() {
  server.handleClient();
  WatchDogCount = 0;
}

void handleIndex() {
  server.send(200, "text/plain", String(sensor_value));
}

void handleUpdate() {
  //data collection from V3B
  sensor_value = server.arg("value");
  Serial.print("Data: "); Serial.println(sensor_value);
  carPower = digitalRead(CarBatteryIN);
  mainPower = digitalRead(MainBatteryIN);
  fuelLid = digitalRead(FuelLid);
  Serial.print("Fuel lid :"); Serial.println(fuelLid);

  if (fuelLid == 1) {
    cut = "1";
  }
  else if (fuelLid == 0) {
    cut = "0";
  }
  else {
    cut = "no";
  }
  delay(20);

  if (carPower == 1 && mainPower == 1) {
    power = "1";
  }
  else if (carPower == 0 && mainPower == 1) {
    power = "0";
  }
  else {
    power = "No";
  }
  delay(20);
  Serial.print("power source :"); Serial.println(power);

  String m = WiFi.macAddress();
  Serial.print("mac address: "); Serial.println(m);

  //data passing through software serial
  String softValue = "";
  softValue += sensor_value + "@" + power + "@" + cut + "@" + m;
  int softValueLength = softValue.length();
  char finalSoftValue[softValueLength];
  strcpy(finalSoftValue, softValue.c_str());
  hostSerial.write(finalSoftValue);

  Serial.print("passing data to gsm :"); Serial.println(finalSoftValue);

  server.send(200, "text/plain", "Updated");
}

#include <SPI.h>
#include <RH_NRF24.h> //NRF library
#include <RTClib.h>  //rtc library
#include <Wire.h>
#include<SoftwareSerial.h>

#define CarBatteryIN 2 //car battery
#define MainBatteryIN 3 //main battery
#define FuelLid 5 //fuel lid

RTC_DS3231 rtc;
RH_NRF24 nrf24(9, 10); //NRF CE AND CSN PIN


SoftwareSerial hostSerial =  SoftwareSerial(6, 7); //Software serial for gsm module  //RX TX

char t[32];
String id = "11-1111";
String percent;
String power;
String cut;
int carPower = 0;
int mainPower = 0;
int fuelLid = 0;
void setup()
{

  Serial.begin(9600);
  hostSerial.begin(9600);
  Wire.begin();
  pinMode(CarBatteryIN, INPUT);
  pinMode(MainBatteryIN, INPUT);
  pinMode(FuelLid, INPUT);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //
  //  Serial.print("compiled: ");
  //  Serial.print(__DATE__);
  //  Serial.print(" ");
  //  Serial.println(__TIME__);


  //NRF initialization
  if (!nrf24.init())
    Serial.println("init failed");
  if (!nrf24.setChannel(2))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");

}


void loop()
{
  carPower = digitalRead(CarBatteryIN);
  mainPower = digitalRead(MainBatteryIN);
  fuelLid = digitalRead(FuelLid);
  Serial.print("Fuel lid :");
  Serial.println(fuelLid);
  
  if (fuelLid == 1) {
    cut = "1";
  }
  else if (fuelLid == 0){
    cut = "0";
  }
  else{
    cut="no";
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
  Serial.print("power source :");
  Serial.println(power);

  //data receive from client
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (nrf24.recv(buf, &len))
  {

    Serial.println("--------------------------------------------------------");
    Serial.println("got request from client");

    //data receive time
    DateTime now = rtc.now();
    sprintf(t, "%02d/%02d/%02d_%02d:%02d:%02d",   now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
    Serial.print("Receive time : ");
    Serial.println(t);

    int batteryStatus = buf[0];
    int sensorValue = buf[1];

    Serial.print("voltage : ");
    Serial.println(batteryStatus);

    Serial.print("sensor value : ");
    Serial.println(sensorValue);


    //    software serial data send
    String softValue = " ";

    softValue += sensorValue;
    softValue += " ";
    softValue += batteryStatus;
    softValue += " ";
    softValue += t;
    softValue += " ";
    softValue += id;
    softValue += " ";
    softValue += power;
    softValue += " ";
    softValue += cut;
    softValue += " ";

    int softValueLength = softValue.length();

    char finalSoftValue[softValueLength + 1];

    strcpy(finalSoftValue, softValue.c_str());

    hostSerial.write(finalSoftValue);
    Serial.print("passing data to gsm :");
    Serial.println(finalSoftValue);
    if (sensorValue == 0) {
      //when cap open
      Serial.println("continue");
      Serial.println("--------------------------------------------------------");
      Serial.println(" ");
    } else {
      //when cap closed
      int i = 0;
      for (i = 0; i < 20; i++) {
        delay(20);
        //sending sleep instraction
        Serial.println("sleep request send");
        uint8_t data[] = "sleep";
        nrf24.send(data, sizeof(data));
        nrf24.waitPacketSent();
      }
      delay(500);
    }
    delay(200);
  }
}

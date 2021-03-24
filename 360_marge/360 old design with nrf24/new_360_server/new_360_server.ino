
#include <SPI.h>
#include <RH_NRF24.h> //NRF library
#include <RTClib.h>  //rtc library
#include <Wire.h>
#include<SoftwareSerial.h>


RH_NRF24 nrf24(9, 10); //NRF CE AND CSN PIN


SoftwareSerial hostSerial =  SoftwareSerial(6, 7); //Software serial for gsm module



void setup()
{
  
  Serial.begin(9600);
  hostSerial.begin(9600);
  Wire.begin();

//NRF initialization
  if (!nrf24.init())
    Serial.println("init failed");
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");

}


void loop()
{
 //data receive from client
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (nrf24.recv(buf, &len))
  {

    Serial.println(" ");
    Serial.println("got request from client");
    
    int batteryStatus = buf[0];
    int sensorValue = buf[1];

    Serial.print("voltage : ");
    Serial.print(batteryStatus);
    Serial.print(" ");
    Serial.print("sensor value : ");
    Serial.println(sensorValue);

    //voltage to battery percent calculation
    int percent = (batteryStatus - 37) * 10;
    Serial.print("battery Status : ");
    Serial.print(percent);
    Serial.println("%");

    //software serial data sending
    char sen[4];
    char bat[4];
    sprintf(sen, "%d", sensorValue);
    sprintf(bat, "%d", percent);
    delay(200);
    hostSerial.write(sen, 4); //Write the serial data for sensor
    hostSerial.write(bat, 4); //Write the serial data for battery


    if (sensorValue == 0) {
      //when cap open
      Serial.print("continue");
      DateTime now = rtc.now();
      sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());

    }
    else {
      //when cap closed
      DateTime now = rtc.now();
      sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());

      int i = 0;
      for (i = 0; i < 20; i++) {
        delay(20);
        //sending sleep instraction
        uint8_t data[] = "sleep";
        nrf24.send(data, sizeof(data));
        nrf24.waitPacketSent();
      }
      delay(500);
    }

    delay(200);
  }

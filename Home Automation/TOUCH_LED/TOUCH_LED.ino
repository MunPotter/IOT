/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender



IRsendRaw mySender;

void setup() {
  Serial.begin(9600);
  delay(2000); 
  while (!Serial); //delay for Leonardo
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
}
/* Cut and paste the output from "rawRecv.ino" below here. It will 
 * consist of a #define RAW_DATA_LEN statement and an array definition
 * beginning with "uint16_t rawData[RAW_DATA_LEN]= {…" and concludes
 * with "…,1000};"
 */
#define RAW_DATA_LEN 100
uint16_t rawDataOff[RAW_DATA_LEN]={
  3338, 1602, 430, 390, 454, 362, 454, 1182, 
  458, 362, 454, 1182, 454, 362, 458, 358, 
  458, 362, 454, 1178, 454, 1182, 458, 362, 
  454, 362, 454, 362, 454, 1178, 454, 1186, 
  430, 386, 450, 366, 426, 390, 426, 390, 
  454, 366, 450, 366, 426, 390, 426, 390, 
  426, 390, 422, 394, 426, 390, 422, 394, 
  426, 394, 422, 1214, 422, 394, 426, 390, 
  398, 418, 422, 394, 398, 418, 398, 422, 
  418, 398, 398, 1238, 398, 422, 394, 422, 
  394, 418, 398, 422, 398, 1238, 398, 418, 
  398, 422, 394, 422, 394, 422, 398, 418, 
  398, 394, 446, 1000};

#define RAW_DATA_LEN 100
uint16_t rawDataOn[RAW_DATA_LEN]={
  3290, 1626, 430, 390, 430, 386, 430, 1206, 
  430, 390, 430, 1206, 458, 358, 430, 390, 
  454, 362, 454, 1178, 454, 1182, 458, 362, 
  454, 362, 458, 358, 458, 1178, 454, 1182, 
  458, 358, 458, 358, 454, 362, 458, 362, 
  454, 362, 454, 362, 454, 362, 454, 362, 
  454, 362, 454, 362, 454, 362, 454, 366, 
  454, 362, 454, 1182, 458, 362, 450, 366, 
  454, 362, 450, 366, 450, 366, 450, 366, 
  454, 362, 454, 1182, 454, 366, 438, 378, 
  454, 362, 426, 390, 426, 1210, 426, 1206, 
  454, 1182, 426, 1206, 430, 1206, 426, 1206, 
  426, 1214, 426, 1000};



/*
 * Cut-and-paste into the area above.
 */
   
void loop() {


  int temp = 25;
  Serial.print("temperature = ");
  Serial.print(temp); 
  Serial.println(" C");
  if (temp > 29) {
    mySender.send(rawDataOn,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
    Serial.println(F("AC Switched On"));
  }
  else if (temp < 26) {
    mySender.send(rawDataOff,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
    Serial.println(F("AC Switched Off"));
  }
  delay(5000);
}

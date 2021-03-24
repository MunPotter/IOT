#include <Adafruit_Fingerprint.h>
#include <ArduinoJson.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

SoftwareSerial mySerial(2, 3);
SoftwareSerial gsmSerial(4, 5);
StaticJsonBuffer<200> jsonBuffer;

byte buff[700] ;
uint8_t b[256];

#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id = 1;
int getFingerprintIDez();

//...............................Setup.........................................

void setup()
{
  Serial.begin(115200);
  gsmSerial.begin(9600);
  DynamicJsonBuffer jsonBuffer;
  delay(1000);

  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }

}

uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

//...................................loop...................................
void loop() {
  Serial.println(id);
 String v = getFingerprintEnroll();
  

//  String str="";
//  int final[300];

  

  Serial.print("fingrprint data: ");
  Serial.println(v);

//  gsm(v);
  deleteDB();
  Serial.print(v);
}


//.........................getFingerprintEnroll function.................

String getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK) Serial.println("Image taken");
    else Serial.print(".");
  }
  p = finger.image2Tz(1);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) Serial.println("Stored!");
  delay(500);
  return uploadFingerpintTemplate();
}


//.........................uploadFingerpintTemplate function.................

String uploadFingerpintTemplate() {
  //  uint8_t rawData;
  uint8_t p = finger.loadModel(1);
  if (p == FINGERPRINT_OK) {
    Serial.print("template "); Serial.print(id); Serial.println(" loaded");
  }
  p = finger.getModel();
  uint8_t templateBuffer[256];

  memset(templateBuffer, 0xff, 256);//zero out template buffer
  
  int index = 0;
  uint8_t arr[256];
  uint32_t starttime = millis();
  while ((index < 256) && ((millis() - starttime) < 1000))
  {
    if (mySerial.available())
    {
      templateBuffer[index] = mySerial.read();
      Serial.println("");
      index++;
    }
  }

  //dump entire templateBuffer.  This prints out 16 lines of 16 bytes
  
  for (int count = 0; count < 16; count++)
  {

    for (int i = 0; i < 16; i++)
    {
     //Serial.print(templateBuffer[count * 16 + i], HEX);
   
//   Serial.print(templateBuffer[count * 16 + i]);
   
    }

  }
//  char arr[100];
  String str="";
  Serial.println(" ");
 for (int m = 0; m <sizeof(templateBuffer); m++) {
  arr[m]=templateBuffer[m];
//  Serial.print(arr[m]);

 str +=arr[m];
 }
// Serial.println("string:");
return str;
 

}
//String str = (char*)arr;






void deleteDB()
{
  finger.emptyDatabase();
  Serial.println("Now database is empty :)");
  delay(1000);
}

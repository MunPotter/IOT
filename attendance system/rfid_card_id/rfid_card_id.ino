#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 16//9
#define SS_PIN 14//10

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
  
void setup() {
   Serial.begin(9600);
   SPI.begin();      // Initiate  SPI bus
   mfrc522.PCD_Init();   // Initiate MFRC522
   Serial.println("swap your card to read");
}


void loop() {
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
   }


   if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
   }

   char str[32] = "";
   array_to_string(mfrc522.uid.uidByte, 4, str); 
   Serial.println(str); //Print the output uid string
   mfrc522.PICC_HaltA();
}



void array_to_string(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}

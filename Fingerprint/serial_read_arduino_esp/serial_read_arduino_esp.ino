#include <SoftwareSerial.h>
SoftwareSerial soft(4, 5);
int buffer;
String finalSoftValue = "";
void setup() {
  Serial.begin(9600);
  soft.begin(9600);
}
void loop() {
  soft.listen();

  while (soft.available() > 0) {
    delay(10);
    char SoftValue = soft.read();
    finalSoftValue += SoftValue;
    Serial.println(finalSoftValue);
  }

}

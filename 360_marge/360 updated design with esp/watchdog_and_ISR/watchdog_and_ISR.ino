#include <Ticker.h>

Ticker Tick;
volatile int WatchDogCount = 0;

void ISRWatchDog() {
  WatchDogCount++;
  if (WatchDogCount > 300) {
    Serial.print("ISRWatchDog function executed...>>");
    ESP.reset();
  }
}

void setup() {
  Serial.begin(115200);
  Tick.attach(1, ISRWatchDog);
}

void loop() {
  Serial.printf("Counter's value increased to ");
  Serial.println(WatchDogCount);
  Serial.println("\n\nVoid LOOP is executing");
  delay(1000);
//  WatchDogCount=0;
//  for(;;);
}

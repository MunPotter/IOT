//#include <SPI.h>
//char buff [50];
//volatile byte indx;
//volatile boolean process;
//char reply [50] = "its sk";
//
//void setup (void) {
//  Serial.begin (9600);
//  pinMode(MISO, OUTPUT);
//  SPCR |= _BV(SPE);
//  indx = 0;
//  process = false;
//  SPI.attachInterrupt();
//}
//
//ISR (SPI_STC_vect)
//{
//  byte c = SPDR;
//  buff [indx++] = c;
//  process = true;
//
//}
//
//void loop (void) {
//  char c;
//  if (process) {
//    process = false;
//    Serial.println (buff);
//    //    Serial.println (reply);
//    indx = 0;
//
//  }
//
//
//  SPDR = 'mun';
//}


#include "SPISlave.h"

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);




  SPISlave.begin();
  SPISlave.setStatus(millis());
}

void loop() {
    SPISlave.onData([](uint8_t * data, size_t len) {
    String message = String((char *)data);
    (void) len;
    if (message.equals("its mun")) {
      char answer[33];
      sprintf(answer, "its sk");
      SPISlave.setData(answer);
    }
//    if (message.equals("its sk")) {
//      char answer[33];
//      sprintf(answer, "its mun");
//      SPISlave.setData(answer);
//    }
    Serial.printf("%s\n", (char *)data);
  });

  }

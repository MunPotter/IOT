#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <VirtualWire.h>

const uint64_t pipeOut1 = 0xE8E8F0F0E1LL;
const uint64_t pipeOut2 = 0xE8E8F0F0E2LL;


RF24 radio(9, 10);

struct MyData {
  int value1;
  int value2;
};
MyData data;


void nrfInitializer() {
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_1MBPS);
  radio.openWritingPipe(pipeOut1);
  radio.openReadingPipe(1, pipeOut2);
}

void setup()
{

  Serial.begin(9600);
  nrfInitializer();
  radio.startListening();
  delay(2);
}




void loop()
{
  
  radio.read(&data, sizeof(MyData));
  Serial.print("sensor value: ");
  Serial.println(data.value1);
  Serial.print("battery status: ");
  Serial.println(data.value2);

}

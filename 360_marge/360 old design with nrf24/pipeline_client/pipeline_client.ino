#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut1 = 0xE8E8F0F0E1LL;
const uint64_t pipeOut2 = 0xE8E8F0F0E2LL;

RF24 radio(9, 10);

struct MyData {
  int value1;
  int value2;
};
MyData data;






void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_1MBPS);
  radio.openWritingPipe(pipeOut2);
  radio.openReadingPipe(1, pipeOut1);
  radio.stopListening();
}


void loop()
{
  data.value1 = 1;
  data.value2 = 100;
  radio.write(&data, sizeof(MyData));
  Serial.println(data.value1);
  Serial.println(data.value2);

}

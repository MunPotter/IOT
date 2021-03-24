
#define NUM_SAMPLES 20
int voltage;
unsigned char sample_count = 0;
int top = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

}

void loop()
{


  while (sample_count < NUM_SAMPLES) {

    voltage += analogRead(A0);
    sample_count++;
    delay(20);
  }
  if (voltage > top) top = voltage / NUM_SAMPLES;
  if (top > h) h = top;
  Serial.print("top voltage ");
  Serial.println(top);
  delay(100);
  sample_count = 0;
  voltage = 0;

}

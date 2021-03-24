const int BUTTON = 4;
int counter = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);

}

void loop()
{
  if (!digitalRead(BUTTON))
  {
    int counter = 0;
    while (!digitalRead(BUTTON)) {
      counter++;
      if (counter == 300){
        Serial.println("i got it");
      }
      Serial.println(counter);
    }
  }
  Serial.println("i got nothing");
}

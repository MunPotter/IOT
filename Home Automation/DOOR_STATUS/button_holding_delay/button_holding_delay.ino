const int BUTTON = 1;
int counter = 0;
void setup()
{
//  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void blinkMe() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

void loop()
{
  if (!digitalRead(BUTTON))
  {
    int counter = 0;
    while (!digitalRead(BUTTON)) {
      delay(100);
      counter++;
      if (counter == 30) {
        blinkMe();blinkMe();blinkMe();
        //Serial.println("i got it");
      }
      //Serial.println(counter);
    }
  }
  //Serial.println("i got nothing");
}

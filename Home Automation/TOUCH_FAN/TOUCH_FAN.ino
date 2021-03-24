const int output4 = 4;
int fSpeed = 0;
const int SENSOR_PIN = 13;


void setup() {
  Serial.begin(115200);
  pinMode(output4, OUTPUT);
  analogWrite(output4, LOW);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
}

void loop() {
  switch (fSpeed) {
    case 0: analogWrite(output4, 0);
      Serial.println("fan speed 0");
      break;
    case 1: analogWrite(output4, 50);
      Serial.println("fan speed 50");
      break;
    case 2: analogWrite(output4, 100);
      Serial.println("fan speed 100");
      break;
    case 3: analogWrite(output4, 200);
      Serial.println("fan speed 200");
      break;
    case 4: analogWrite(output4, 255);
      Serial.println("fan speed 255");
      break;
    case 5: analogWrite(output4, 0);
      Serial.println("fan speed 0 and end");
      fSpeed = 0;
      break;
  }

  if (digitalRead(SENSOR_PIN) == HIGH) {
    while (digitalRead(SENSOR_PIN)) {
      delay(50);
    }
    if (fSpeed <= 4) fSpeed++;
  }
}

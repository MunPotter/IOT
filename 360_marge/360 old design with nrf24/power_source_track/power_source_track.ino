#define CarBatteryIN 2 //car battery
#define MainBatteryIN 3 //main battery

String power;
int carPower = 0;
int mainPower = 0;

void setup() {
  Serial.begin(9600);
  pinMode(CarBatteryIN, INPUT);
  pinMode(MainBatteryIN, INPUT);
}


void loop() {
  carPower = digitalRead(CarBatteryIN);

  mainPower = digitalRead(MainBatteryIN);
  Serial.print("car:");
  Serial.print(carPower);
  Serial.print("    ");
  Serial.print("main:");
  Serial.println(mainPower);



  if (carPower == 1 && mainPower == 1) {
    power = "car battery connected";

  }
  else if (carPower == 1 && mainPower == 0) {
    power = "car battery disconnected &  battery connected";
  }
  else {
    power = "No battery connected";
  }
    Serial.print("power source :");
    Serial.println(power);
  delay(200);

}

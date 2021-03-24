void setup() {

  //  Serial.begin(9600);
  pinMode(SensorPin, INPUT_PULLUP);
  //  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, INPUT_PULLUP);
  EEPROM.begin(512);
  Tick.attach(1, ISRWatchDog);

  //wifi settings
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  //  wifiManager.resetSettings();
  String wifimac = WiFi.macAddress();
  String defaults = "rokkhi_";
  String rokkhi = defaults + wifimac;

  char mmmm [20];
  rokkhi.toCharArray(mmmm, 30);

  if (!wifiManager.autoConnect(mmmm, mmmm)) {
    delay(3000);
    ESP.restart();
    delay(1000);
  }
  char eeprom = char(EEPROM.read(addr));
  if (eeprom == '0') {
    //  send to server
    SendToServer();
    delay(200);
  }
  EEPROM.write(addr, '0');
  EEPROM.commit();
  WatchDogCount = 0;
  //  printMe("going to deep sleep");
  ESP.deepSleep(0);

}

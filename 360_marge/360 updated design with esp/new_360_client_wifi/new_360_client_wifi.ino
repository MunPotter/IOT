#include <FS.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>


int sensorValue = 0;
float battery_voltage;

ADC_MODE(ADC_VCC);

//const char* ssid = "YOUR_SSID";
//const char* password = "YOUR_PASSWORD";

const char* host = "192.168.11.4";

WiFiClient client;


void setup() {

  pinMode(5, INPUT_PULLUP);
  //  WiFi.begin(ssid, password);
  //
  //  Serial.begin(115200);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    Serial.print(".");
  //    delay(500);
  //  }

  //wifi settings
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  //    wifiManager.resetSettings();

  if (!wifiManager.autoConnect("rokkhi_client@", "rokkhi_client@")) {
    delay(3000);
    ESP.restart();
    delay(1000);
  }

  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" ");

  sensorValue = digitalRead(5);
  delay(5);
  Serial.print("sensor value: "); Serial.println(sensorValue);

  battery_voltage = ((float)ESP.getVcc()) / 1024;
  Serial.print("battery status: "); Serial.println(battery_voltage);

  String m = WiFi.macAddress();
  Serial.print("mac address: "); Serial.println(m);


  String g = "";
  String f = "";
  f += sensorValue;
  g += battery_voltage;
  String total = "";
  total += f + "@" + g + "@" + m;
  if (client.connect(host, 80)) {
    String url = "/update?value=";
    url += total;
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" +
                 "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
    delay(10);
    while (client.available()) {
      String line = client.readStringUntil('\r');
    }
  }

  Serial.println("Device in sleep mode");
  ESP.deepSleep(0);

}

void loop() {


}

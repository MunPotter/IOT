
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>




IPAddress ip(192, 168, 11, 4);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 255, 0);

// Set up the server object
ESP8266WebServer server;

// Keep track of the sensor data that's going to be sent by the client
String sensor_value = "";

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  String m = WiFi.macAddress();
  String ssid_client = "";
  ssid_client = "rokkhi_server@"+m;
  String ssid_ap = ssid_client;
  String password_ap = ssid_client;
  
  WiFi.softAP(ssid_ap, password_ap);
  // Print IP Address as a sanity check
  Serial.begin(115200);
  Serial.println();

  // Configure the server's routes
  server.on("/", handleIndex); // use the top root path to report the last sensor value
  server.on("/update", handleUpdate); // use this route to update the sensor value
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handleIndex() {
  server.send(200, "text/plain", String(sensor_value)); // we'll need to refresh the page for getting the latest value
}

void handleUpdate() {
  sensor_value = server.arg("value");
  Serial.print("Data: ");
  Serial.println(sensor_value);
  server.send(200, "text/plain", "Updated");
}

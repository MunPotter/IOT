String SendToServer() {
  String m = WiFi.macAddress();
  Serial.println(m);
  String w = WiFi.SSID();
  Serial.println(w);
  String ip = WiFi.localIP().toString();

  String door;
  sensor_status = analogRead(SensorPin);
  if (sensor_status == 0) {
    door = "close";
  }
  else {
    door = "open";
  }
  battery_voltage = ((float)ESP.getVcc()) / 1024;

  String f = "";
  String g = "";

  f += door;
  g += battery_voltage;

  //  auth = httpGETRequest(serverName3);
  //  Serial.println(auth);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    //prepare JSON data from
    //    String z = "{\"fingerprintIdOnDevice\":\"" + f + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    String z = "{\"doorStatus\":\"" + f + "\",\"batteryStatus\":\"" + g + "\",\"SSID\":\"" + w + "\",\"ipAddress\":\"" + ip + "\",\"macAddressOfDevice\":\"" + m + "\"}";
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    //    http.addHeader("authtoken", auth);

    //    Serial.print(z);
    int httpResponseCode = http.POST(z);
    delay(200);
    //    Serial.print("HTTP Response code for registration: ");
    //    Serial.println(httpResponseCode);
    if ((httpResponseCode == 201) || (httpResponseCode == 200)) {
      //      Serial.println("http: success");
      blinkMe(); blinkMe(); blinkMe();
      delay(1000);
    }
    else {
      //      Serial.println("http: failed");
      blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe();
      delay(1000);
    }
    http.end();
  }
  else {
    blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe(); blinkMe();
  }
}

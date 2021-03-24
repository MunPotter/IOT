// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid     = "RokkhiTechTPLink";
const char* password = "RokkhiTech2021";

// Set web server port number to 80
WiFiServer server(80);


// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String lightPin1State = "off";
String lightPin2State = "off";
String fanPinState = "off";
// Assign output variables to GPIO pins
const int lightPin1 = 2;//light
const int lightPin2 = 5;//light
const int fanPin = 4;//fan


const int SENSOR_PIN = 14;//light
const int SENSOR_PIN2 = 12;//light
const int SENSOR_PIN3 = 13;//fan

int fSpeed = 0;
int lastState = 0;
int currentState;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(lightPin1, OUTPUT);
  pinMode(lightPin2, OUTPUT);
  pinMode(fanPin, OUTPUT);
  // Set outputs to LOW
  digitalWrite(lightPin1, LOW);
  digitalWrite(lightPin2, LOW);
  analogWrite(fanPin, LOW);


  pinMode(SENSOR_PIN, INPUT);
  pinMode(SENSOR_PIN2, INPUT);
  pinMode(SENSOR_PIN3, INPUT_PULLUP);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {

  if (digitalRead(SENSOR_PIN) == HIGH) {
    if (digitalRead(lightPin1) == HIGH) {
      digitalWrite(lightPin1, LOW);
      lightPin1State = "off";
    }
    else {
      digitalWrite(lightPin1, HIGH);
      lightPin1State = "on";
    }
    while (digitalRead(SENSOR_PIN)) {
      delay(50);
      Serial.println("user holding the sensor");
    }
  }

  if (digitalRead(SENSOR_PIN2) == HIGH) {
    if (digitalRead(lightPin2) == HIGH) {
      digitalWrite(lightPin2, LOW);
      lightPin2State = "off";
    }
    else {
      digitalWrite(lightPin2, HIGH);
      lightPin2State = "on";
    }
    while (digitalRead(SENSOR_PIN2)) {
      delay(50);
      Serial.println("user holding the sensor");
    }
  }


  switch (fSpeed) {
    case 0: analogWrite(fanPin, 0);
      Serial.println("fan speed 0");
      break;
    case 1: analogWrite(fanPin, 50);
      Serial.println("fan speed 50");
      fanPinState = "on";
      break;
    case 2: analogWrite(fanPin, 100);
      Serial.println("fan speed 100");
      break;
    case 3: analogWrite(fanPin, 200);
      Serial.println("fan speed 200");
      break;
    case 4: analogWrite(fanPin, 255);
      Serial.println("fan speed 255");
      break;
    case 5: analogWrite(fanPin, 0);
      Serial.println("fan speed 0 and end");
      fanPinState = "off";
      fSpeed = 0;
      break;
  }

  if (digitalRead(SENSOR_PIN3) == HIGH) {
    while (digitalRead(SENSOR_PIN3)) {
      delay(50);
    }
    if (fSpeed <= 4) fSpeed++;
  }


  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:


            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              Serial.println("LED 1 on");
              lightPin1State = "on";
              digitalWrite(lightPin1, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              Serial.println("LED 1 off");
              lightPin1State = "off";
              digitalWrite(lightPin1, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              Serial.println("LED 2 on");
              lightPin2State = "on";
              digitalWrite(lightPin2, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              Serial.println("LED 2 off");
              lightPin2State = "off";
              digitalWrite(lightPin2, LOW);
            }
            else if (header.indexOf("GET /6/on") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              Serial.println("FAN on");
              analogWrite(fanPin, 50);
              fSpeed = 1;
              fanPinState = "on";
            }
            else if (header.indexOf("GET /6/off") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              Serial.println("FAN off");
              analogWrite(fanPin, 0);
              fSpeed = 0;
              fanPinState = "off";
            }
            else if (header.indexOf("GET /status") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:application/json");
              client.println("Connection: close");
              client.println();
              String m = WiFi.macAddress();
              String pin1 = "5";
              String pin2 = "2";
              String pin3 = "4";
              String z = "{\"device\":\"" + m + "\",\"status\":[{\"pin\":\"" + pin1 + "\",\"state\":\"" + lightPin2State + "\"},{\"pin\":\"" + pin2 + "\",\"state\":\"" + lightPin1State + "\"},{\"pin\":\"" + pin3 + "\",\"state\":\"" + fanPinState + "\"}\]\}";

              client.println(z);
              client.stop();
            }


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button {background-color: #ff3300; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #ff3300;}</style></head>");


            // Web Page Heading
            client.println("<img src=\"https://www.rokkhi.com/img/rlogo.png\" style=vertical-align:middle  width=100 height=100>");
//            client.println("<img src=\"https://rokkhi.com/img/logo.png\" style=vertical-align:middle  width=150 height=100>");
            client.println("<body><h1>Rokkhi Home Automation</h1>");

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<h3>LED 1 - State " + lightPin1State + "</h3>");
            // If the lightPin1State is off, it displays the ON button
            if (lightPin1State == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">LED ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">LED OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<h3>LED 2 - State " + lightPin2State + "</h3>");
            // If the lightPin2State is off, it displays the ON button
            if (lightPin2State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">LED ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">LED OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<h3>FAN - State " + fanPinState + "</h3>");
            // If the lightPin2State is off, it displays the ON button
            if (fanPinState == "off") {
              client.println("<p><a href=\"/6/on\"><button class=\"button\">FAN ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/6/off\"><button class=\"button button2\">FAN OFF</button></a></p>");
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

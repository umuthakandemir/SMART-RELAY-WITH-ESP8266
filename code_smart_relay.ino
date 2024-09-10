#include <ESP8266WiFi.h>

const char* ssid = "ssid";
const char* password = "password";

const int relayPin = 2; // GPIO2 is connected to the relay module

WiFiServer server(1521);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  server.begin();

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Initialize relay as off
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/RELAY=ON") != -1) {
    digitalWrite(relayPin, LOW);
  } else if (request.indexOf("/RELAY=OFF") != -1) {
    digitalWrite(relayPin, HIGH);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("Relay is now " + String(digitalRead(relayPin) == LOW ? "OFF" : "ON"));
  client.println("</html>");
}

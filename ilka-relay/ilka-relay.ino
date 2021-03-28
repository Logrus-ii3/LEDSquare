#include "config.h"
#include <ESP8266WiFi.h>

int port = 8888;  //Port number
const int relayPin = D1;

WiFiServer server(port);
WiFiClient client;

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  delay(50);
  Serial.begin(115200);
  setup_wifi();
  pinMode(relayPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  server.begin();
}

void loop() {
  if (!client.connected()) {
    client = server.available();
  } else {
    if (client.available() > 0) {
      // read data from the connected client
      char msgChar = client.read();
      if (msgChar == '0') {
        digitalWrite(relayPin, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        client.stop();
      } else if (msgChar == '1') {
        digitalWrite(relayPin, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        client.stop();
      }
    }
  }
}

#include "config.h"
#include "timer.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// LED parameters
const int redPin = D5;
const int greenPin = D6;
const int bluePin = D7;
const int maxAllowedBrightness = 512;
const int maxMessageLength = 255;

char messageBuffer[maxMessageLength];

WiFiClient espClient;
PubSubClient client(espClient);
Timer timer = Timer();

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

void setBrightness(int red, int green, int blue) {
  timer.finish();
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Sets white brightness
void setBrightness(int value) {
  setBrightness(value, value, value);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (length > maxMessageLength) {
    Serial.println("Message was too large");
    return;
  }
  memcpy(messageBuffer, payload, length);
  messageBuffer[length] = 0;
  processMessage();
}

// Analyze message
void processMessage() {
  if (strcmp(messageBuffer, "on") == 0 || strcmp(messageBuffer, "white") == 0) {
    setBrightness(maxAllowedBrightness);
    return;
  }
  if (strcmp(messageBuffer, "off") == 0) {
    setBrightness(0);
    return;
  }
  if (strcmp(messageBuffer, "night") == 0) {
    setBrightness(50, 50, 10);
    return;
  }

  char* result = strtok(messageBuffer, " ");
  if (result != NULL) {
    if (strcmp(result, "timer") == 0) {
      result = strtok(NULL, " ");
      if (result != NULL) {
        timer.start(atoi(result) * 1000);
      }
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection... ");
    // Create a random client ID
    String clientId = MQTT_CLIENT_ID;
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      // resubscribe
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  messageBuffer[0] = 0;
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setBrightness(0);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (timer.doStep() == 1) {
    setBrightness(0);
  }
}

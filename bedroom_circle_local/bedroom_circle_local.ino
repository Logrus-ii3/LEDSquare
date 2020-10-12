#include "config.h"
#include "timer.h"
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const int maxMessageLength = 255;
char messageBuffer[maxMessageLength];
const int DHTPin = D2;

// LED parameters
const int redPin = D5;
const int greenPin = D6;
const int bluePin = D7;
const int maxAllowedBrightness = 512;

WiFiClient espClient;
PubSubClient client(espClient);
Timer timer = Timer();
DHT dht(DHTPin, DHTTYPE);
#include "dhtReader.h"
DhtReader dhtReader(DEFAULT_INTERVAL);
boolean hasState = true;
boolean hasInterval = true;

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
  Serial.print("Setting leds RGB: ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(blue);
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
  if (strcmp(topic, MQTT_COMMAND_TOPIC) == 0) {
    processMessage();
    return;
  }
  if (strcmp(topic, MQTT_STATE_TOPIC) == 0) {
    readStateSetting();
  }
  if (strcmp(topic, MQTT_INTERVAL_TOPIC) == 0) {
    readIntervalSetting();
  }
}

// Analyze message
void processMessage() {
  if (strcmp(messageBuffer, "on") == 0 || strcmp(messageBuffer, "white") == 0) {
    setBrightness(maxAllowedBrightness);
    client.publish(MQTT_STATE_TOPIC, "on", true);
    return;
  }
  if (strcmp(messageBuffer, "off") == 0) {
    setBrightness(0);
    client.publish(MQTT_STATE_TOPIC, "off", true);
    return;
  }
  if (strcmp(messageBuffer, "night") == 0) {
    setBrightness(50, 50, 10);
    client.publish(MQTT_STATE_TOPIC, "night", true);
    return;
  }
  if (strcmp(messageBuffer, "restart") == 0) {
    Serial.println("Restarting...");
    ESP.restart();
    return;
  }

  char* result = strtok(messageBuffer, " ");
  if (result != NULL) {
    if (strcmp(result, "timer") == 0) {
      result = strtok(NULL, " ");
      if (result != NULL) {
        int t = atoi(result);
        if (t > 0) {
          timer.start(atoi(result) * 1000);
        }
      }
    }
    if (strcmp(result, "interval") == 0) {
      result = strtok(NULL, " ");
      if (result != NULL) {
        unsigned long interval = strtol(result, 0, 10);
        Serial.print("Got interval: ");
        Serial.println(interval);
        if (interval == 0) {
          interval = DEFAULT_INTERVAL;
        }
        dhtReader.setInterval(interval);
        client.publish(MQTT_INTERVAL_TOPIC, ltoa(interval, messageBuffer, 10), true);
      }
    }
    if (strcmp(result, "rgb") == 0) {
      result = strtok(NULL, " ");
      if (result != NULL) {
        int red = atoi(result);
        result = strtok(NULL, " ");
        if (result != NULL) {
          int green = atoi(result);
          result = strtok(NULL, " ");
          if (result != NULL) {
            int blue = atoi(result);
            setBrightness(red, green, blue);
            client.publish(MQTT_STATE_TOPIC, messageBuffer, true);
          }
        }
      }
    }
  }
}

void readIntervalSetting() {
  unsigned long interval = strtol(messageBuffer, 0, 10);
  Serial.print("Got interval: ");
  Serial.println(interval);
  if (interval == 0) {
    interval = DEFAULT_INTERVAL;
  }
  dhtReader.setInterval(interval);
  client.unsubscribe(MQTT_INTERVAL_TOPIC);
}

void readStateSetting() {
  Serial.println("Got settings.");
  if (strcmp(messageBuffer, "on") == 0) {
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

  char* pNext;
  int red = strtol(messageBuffer, &pNext, 10);
  int green = strtol(pNext, &pNext, 10);
  int blue = strtol(pNext, &pNext, 10);
  setBrightness(red, green, blue);
  client.unsubscribe(MQTT_STATE_TOPIC);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection... ");
    // Create client ID
    String clientId = MQTT_CLIENT_ID;
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      // resubscribe
      client.subscribe(MQTT_STATE_TOPIC);
      client.subscribe(MQTT_INTERVAL_TOPIC);
      client.subscribe(MQTT_COMMAND_TOPIC);
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
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  dhtReader.loop();

  if (timer.doStep() == 1) {
    setBrightness(0);
    client.publish(MQTT_STATE_TOPIC, "off", true);
    timer.finish();
  }
//  ESP.wdtFeed(); // feeds the dog
//  delay(0);
}

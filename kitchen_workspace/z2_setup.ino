void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(D0, LOW);
    delay(250);
    digitalWrite(D0, HIGH);
    delay(250);
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  delay(100);
  Serial.begin(115200);
  messageBuffer[0] = 0;

  pinMode(D0, OUTPUT);
  digitalWrite(D0, builtin);

  FastLED.addLeds<WS2812, PIN_SMALL>(ledsSmall, NUM_LEDS_SMALL);
  FastLED.addLeds<WS2812, PIN_BIG>(ledsBig, NUM_LEDS_BIG);
  LEDS.setBrightness(50);
  irrecv.enableIRIn();  // Start the receiver

  setup_wifi();

  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  //worker.start();
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
      client.publish(MQTT_IP_TOPIC, WiFi.localIP().toString().c_str(), true);
      client.subscribe(MQTT_STATE_TOPIC);
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

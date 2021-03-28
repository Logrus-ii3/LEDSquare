// Commands
void processCommand() {
  // Simple messages
  if (strcmp(messageBuffer, "off") == 0) {
    worker.runColor(0, 0, 0);
    client.publish(MQTT_STATE_TOPIC, "off", true);
    return;
  }
  if (strcmp(messageBuffer, "red") == 0) {
    worker.runColor(255, 0, 0);
    client.publish(MQTT_STATE_TOPIC, "red", true);
    return;
  }
  if (strcmp(messageBuffer, "on") == 0) {
    worker.stop();
    setAllColor(maxBrightness, maxBrightness, maxBrightness);
    client.publish(MQTT_STATE_TOPIC, "on", true);
    return;
  }
  if (strcmp(messageBuffer, "newYearFading") == 0) {
    worker.setEffect(0);
    client.publish(MQTT_STATE_TOPIC, "newYearFading", true);
    return;
  }
  if (strcmp(messageBuffer, "girland") == 0) {
    worker.setEffect(1);
    client.publish(MQTT_STATE_TOPIC, "girland", true);
    return;
  }
  if (strcmp(messageBuffer, "rainbow") == 0) {
    worker.setEffect(2);
    client.publish(MQTT_STATE_TOPIC, "rainbow", true);
    return;
  }

  // Complex messages
  char* result = strtok(messageBuffer, " ");
  if (result != NULL) {
    if (strcmp(result, "brightness") == 0) {
      result = strtok(NULL, " ");
      if (result != NULL) {
        int t = atoi(result);
        if (t > 0 && t < 255) {
          LEDS.setBrightness(t);
          showLeds();
          client.publish(MQTT_BRIGHTNESS_TOPIC, itoa(t, messageBuffer, 10), true);
        }
      }
    }
  }
}

// MQTT Utils functions
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
    processCommand();
    return;
  }
}

void onRoot() {
  Serial.println("Root was requested");
  server.send(200, "text/plain", "OK");
}

void onGetModes() {
  Serial.println("Sending modes.");
  server.send(200, "text/plain", effects.getModes());
}

void onMode() {
  int currentMode = getArg("value").toInt();
  Serial.print("Setting mode: ");
  Serial.println(currentMode);
  effects.setMode(currentMode);
  sendOk();
}

void onBrightness() {
  int brightness = getArg("value").toInt();
  if (brightness > 0) {
    Serial.print("Setting brightness: ");
    Serial.println(brightness);
    FastLED.setBrightness(brightness);
    sendOk();
  } else {
    server.send(500, "text/plain", "Unknown value: " + getArg("value"));
  }
}

void onSpeed() {
  long effectTickDelay = getArg("value").toInt();
  if (effectTickDelay > 0) {
    Serial.print("Setting speed: ");
    Serial.println(effectTickDelay);
    effects.setTickDelay(effectTickDelay);
  } else {
    server.send(500, "text/plain", "Unknown value: " + getArg("value"));
  }
}

void onScale() {
  long scale = getArg("value").toInt();
  if (scale > 0) {
    Serial.print("Setting scale: ");
    Serial.println(scale);
    effects.setScale(scale);
  } else {
    server.send(500, "text/plain", "Unknown value: " + getArg("value"));
  }
}

void sendOk() {
  server.send(200, "text/plain", "OK");
}

String getArg(String key) {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == key) {
      return server.arg(i);
    }
  }
  return "";
}

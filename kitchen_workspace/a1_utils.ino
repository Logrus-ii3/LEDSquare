void setAllColor(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i][0] = red;
    leds[i][1] = green;
    leds[i][2] = blue;
  }
  showLeds();
}

void stepDimAll() {
  int brightness = LEDS.getBrightness();
  brightness -= DIM_STEP;
  if (brightness < 0) {
    brightness = 0;
  }
  LEDS.setBrightness(brightness);
  showLeds();
  client.publish(MQTT_BRIGHTNESS_TOPIC, itoa(brightness, messageBuffer, 10), true);
}

void stepBrightenAll() {
  int brightness = LEDS.getBrightness();
  brightness += DIM_STEP;
  if (brightness > 255) {
    brightness = 255;
  }
  LEDS.setBrightness(brightness);
  showLeds();
  client.publish(MQTT_BRIGHTNESS_TOPIC, itoa(brightness, messageBuffer, 10), true);
}

void showLeds() {
  // Pay attention to GRB instead of RGB
  for (int i = 0; i < NUM_LEDS_SMALL; i++) {
    ledsSmall[i].g = leds[i][0];
    ledsSmall[i].r = leds[i][1];
    ledsSmall[i].b = leds[i][2];
  }
  for (int i = 0; i < NUM_LEDS_BIG; i++) {
    ledsBig[i].g = leds[i+NUM_LEDS_SMALL][0];
    ledsBig[i].r = leds[i+NUM_LEDS_SMALL][1];
    ledsBig[i].b = leds[i+NUM_LEDS_SMALL][2];
  }
  FastLED.show();
  invertBuiltin();
}

void invertBuiltin() {
  builtin = !builtin;
  digitalWrite(D0, builtin);
}

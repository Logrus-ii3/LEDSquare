int brightness = 10;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  worker.tick();
  if (irrecv.decode(&irResults)) {
    if (irResults.value == BLACK_IR_BUTTON_1) {
      worker.runColor(maxBrightness, 0, 0);
      client.publish(MQTT_STATE_TOPIC, "red", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_2) {
      worker.runColor(0, maxBrightness, 0);
      client.publish(MQTT_STATE_TOPIC, "green", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_3) {
      worker.runColor(0, 0, maxBrightness);
      client.publish(MQTT_STATE_TOPIC, "blue", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_4) {
      worker.setEffect(0);
      client.publish(MQTT_STATE_TOPIC, "newYearFading", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_5) {
      worker.setEffect(1);
      client.publish(MQTT_STATE_TOPIC, "girland", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_6) {
      worker.setEffect(2);
      client.publish(MQTT_STATE_TOPIC, "rainbow", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_0) {
      worker.runColor(maxBrightness, maxBrightness, maxBrightness);
      client.publish(MQTT_STATE_TOPIC, "on", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_SHARP) {
      worker.runColor(0, 0, 0);
      client.publish(MQTT_STATE_TOPIC, "off", true);
    }
    if (irResults.value == BLACK_IR_BUTTON_UP) {
      stepBrightenAll();
    }
    if (irResults.value == BLACK_IR_BUTTON_DOWN) {
      stepDimAll();
    }
    if (irResults.value == BLACK_IR_BUTTON_LEFT) {
      worker.setStepDelay(worker.getStepDelay() + 10);
      invertBuiltin();
    }
    if (irResults.value == BLACK_IR_BUTTON_RIGHT) {
      worker.setStepDelay(worker.getStepDelay() - 10);
      invertBuiltin();
    }
    irrecv.resume();  // Receive the next value
  }
}

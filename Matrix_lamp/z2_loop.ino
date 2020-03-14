void loop() {
  effects.tick();
  server.handleClient();
  yield();
}

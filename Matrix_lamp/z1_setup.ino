void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS);
  delay(100);
  Serial.print("Brightness: ");
  Serial.println(FastLED.getBrightness());
  FastLED.setBrightness(5);
  Serial.print("Brightness: ");
  Serial.println(FastLED.getBrightness());
  ledsMem[15][15] = CRGB(1, 0, 0);
  flushLedsMem();

  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  boolean off = false;
  byte x = 0;
  while (WiFi.status() != WL_CONNECTED) {
    ledsMem[x][15] = CRGB(0, 0, 5);
    flushLedsMem();
    Serial.print(".");
    delay(100);
    ledsMem[x][15] = bgColor;
    x = x + 1;
    if (x == WIDTH) {
      x = 0;
    }
  }
  Serial.println(" Success");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP().toString());

  Serial.print("Updating time...");
  timeClient.begin();
  while(!timeClient.forceUpdate()) {
    ledsMem[x][15] = CRGB(0, 5, 0);
    flushLedsMem();
    Serial.print(".");
    delay(100);
    ledsMem[x][15] = bgColor;
    x = x + 1;
    if (x == WIDTH) {
      x = 0;
    }
  }
  setTime(timeClient.getEpochTime());
  Serial.println(" Success");

  Serial.print("Setting up and starting web server...");
  setupServer();
  server.begin();
  Serial.println(" Success");

  startUpAnimation();
  //testRGB();
}

void setupServer() {
  server.on("/", onRoot);
  server.on("/modes", onGetModes);
  server.on("/mode", onMode);
  server.on("/brightness", onBrightness);
  server.on("/speed", onSpeed);
  server.on("/scale", onScale);

  server.onNotFound([]() {
      String message = "File Not Found\n\n";
      message += "URI: ";
      message += server.uri();
      message += "\nMethod: ";
      message += (server.method() == HTTP_GET) ? "GET" : "POST";
      message += "\nArguments: ";
      message += server.args();
      message += "\n";
      for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      }
      server.send(404, "text/plain", message);
      Serial.println(message);
    }
  );
}

void startUpAnimation() {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      ledsMem[x][y] = CRGB(255, 0, 0);
    }
    flushLedsMem();
    for (int x = 0; x < WIDTH; x++) {
      ledsMem[x][y] = CRGB(0, 0, 0);
    }
    delay(20);
  }
}

void testRGB() {
  ledsMem[0][0] = CRGB(255, 0, 0);
  ledsMem[1][0] = CRGB(0, 255, 0);
  ledsMem[2][0] = CRGB(0, 0, 255);
  
  ledsMem[0][1] = 0xFF0000;
  ledsMem[1][1] = 0x00FF00;
  ledsMem[2][1] = 0x0000FF;
  flushLedsMem();
  delay(2000);
  blackOut();
}

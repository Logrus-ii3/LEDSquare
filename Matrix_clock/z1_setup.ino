//Particle particle(CRGB(0, 0, 3), CRGB(0, 0, 0), 50, 5);
//Particle particle2(CRGB(0, 3, 0), CRGB(0, 0, 0), 60, 1);
//Particle particle3(CRGB(0, 3, 3), CRGB(0, 0, 0), 70, 3);

// Update weather every 10 mins
Weather weather(10*60*1000);

// Flashes BLUE while connecting WiFi
// Flashes GREEN while updating time
void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2811, PIN>(leds, NUM_LEDS);
  ledsMem[15][15] = CRGB(0, 5, 0);
  flushLedsMem();

  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  boolean off = false;
  while (WiFi.status() != WL_CONNECTED) {
    if (off) {
      ledsMem[15][15] = CRGB(0, 0, 5);
    } else {
      ledsMem[15][15] = CRGB(0, 0, 0);
    }
    off = !off;
    flushLedsMem();
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Success");

  Serial.print("Updating time");
  timeClient.begin();
  while(!timeClient.forceUpdate()) {
    if (off) {
      ledsMem[15][15] = CRGB(5, 0, 0);
    } else {
      ledsMem[15][15] = CRGB(0, 0, 0);
    }
    off = !off;
    flushLedsMem();
    Serial.print(".");
    delay(500);
  }
  ledsMem[15][15] = CRGB(0, 0, 0);
  setTime(timeClient.getEpochTime());
  Serial.println(" Success");

  weather.doUpdate();

  Serial.println("Setup end");
}

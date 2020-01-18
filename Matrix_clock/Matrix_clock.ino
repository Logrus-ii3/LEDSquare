#define FASTLED_ESP8266_RAW_PIN_ORDER

#include "FastLED.h"
#include "config.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include "fonts.h"

#define NUM_LEDS 256
#define PIN D1
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];
CRGB ledsMem[16][16]; // x, y
CRGB bgColor = CRGB(0, 0, 0);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, UTC_OFFSET_IN_SECONDS);

void loop_check() {
  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 16; j++) {
      ledsMem[j][i] = CRGB(5, 0, 0);
      flushLedsMem();
      delay(50);
      ledsMem[j][i] = CRGB(0, 0, 0);
    }
  }
}

void loop_pre() {
  for(int x = 0; x < NUM_LEDS; x++){
    leds[x] = CRGB(0,0,20); // g,r,b
    FastLED.show();
    delay(50);
    leds[x] = CRGB(0,0,0);
  }
}

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#define NUM_LEDS 256
#define PIN D1
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<WS2811, PIN>(leds, NUM_LEDS);
}
void loop() {
  for(int x = 0; x < NUM_LEDS; x++){
    leds[x] = CRGB(0,0,200); // g,r,b
    FastLED.show();
    delay(50);
    leds[x] = CRGB(0,0,0);
  }
}

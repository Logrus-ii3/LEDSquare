#define FASTLED_ESP8266_RAW_PIN_ORDER

//#include "colorpalettes.h"
#include <FastLED.h>
#include "config.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

#define NUM_LEDS 256
#define PIN D1

CRGB leds[NUM_LEDS];
CRGB ledsMem[16][16]; // x, y
CRGB bgColor = CRGB(0, 0, 0);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, UTC_OFFSET_IN_SECONDS);
ESP8266WebServer server(80);

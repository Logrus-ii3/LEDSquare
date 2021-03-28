#include "config.h"
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int maxMessageLength = 255;
char messageBuffer[maxMessageLength];

WiFiClient espClient;
PubSubClient client(espClient);

IRrecv irrecv(IR_RECV_PIN);
decode_results irResults;

CRGB ledsBig[NUM_LEDS_BIG];
CRGB ledsSmall[NUM_LEDS_SMALL];
int leds[NUM_LEDS][3];

int maxBrightness = 255; // Deprecated
int builtin = HIGH;

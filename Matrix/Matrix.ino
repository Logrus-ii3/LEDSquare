#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "config.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include "FastLED.h"
#define NUM_LEDS 256
#define PIN D1
#define COLOR_ORDER RGB

AdafruitIO_Feed *matrix = io.feed("matrix");
CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<WS2811, PIN>(leds, NUM_LEDS);
  delay(1000);
  Serial.begin(115200);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  matrix->onMessage(loadFile);
  matrix->get();
  //loadFile("http://192.168.1.15/mario/mario-stand.bmp");
}

void loop() {
  io.run();
}

void loadFile(AdafruitIO_Data *data) {
  String url = data->value();
  Serial.print("Got URL: ");
  Serial.println(url);
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  int len = http.getSize();

  uint8_t buff[len];
  WiFiClient * stream = http.getStreamPtr();
  while(http.connected() && (len > 0 || len == -1)) {
    int c = stream->readBytes(buff, len);
  }
  //printHexData(buff, len);
  int32_t offset = readNbytesInt(buff, 10, 4);
  Serial.println(offset);
  showPicture(buff, offset);
}

void showPicture(uint8_t* buff, int offset) {
  for(int j = 0; j < 8; j++) {
    for(int i = 0; i < 16; i++) {
      leds[j*32+i] = getPixelColor(buff, offset, j*32+i);
      leds[(j*32+i+16)+(32-(i+16)-(i+16-15))] = getPixelColor(buff, offset, j*32+i+16);
    }
  }
  FastLED.show();
}

CRGB getPixelColor(uint8_t* buff, int offset, int i) {
    int pixel = 3 * i;
    byte b = buff[offset+pixel];
    byte g = buff[offset+pixel+1];
    byte r = buff[offset+pixel+2];
    //Serial.printf("%d:(%d,%d,%d)", i, r, g, b);
    //Serial.println();
    return CRGB(g, r, b);
}

int32_t readNbytesInt(uint8_t buf[], int position, byte nBytes) {
  if (nBytes > 4)
    return 0;

  int32_t weight = 1;
  int32_t result = 0;
  int i = position;
  for (; nBytes; nBytes--) {
    int b = buf[i++];
    //Serial.println(b);
    result += weight * b;
    weight <<= 8;
  }
  return result;
}

void printHexData(uint8_t buf[], int len) {
  for(int i = 0; i < len; i++) {
    if (i % 16 == 0) {
      Serial.println(i);
    }
    Serial.print(buf[i], HEX);
    Serial.print("|");
  }
}

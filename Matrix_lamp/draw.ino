// **************** НАСТРОЙКА МАТРИЦЫ ****************
#define WIDTH 16
#define HEIGHT 16
#define MATRIX_TYPE 0
//#define CONNECTION_ANGLE 1
//#define STRIP_DIRECTION 3
#define CONNECTION_ANGLE 3
#define STRIP_DIRECTION 2

#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y (WIDTH - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y (WIDTH - x - 1)

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif

void flushLedsMem() {
  for (byte y = 0; y < 16; y++) {
    for (byte x = 0; x < 16; x++) {
      if ((THIS_Y % 2 == 0) || MATRIX_TYPE) {
        // если чётная строка
        leds[THIS_Y * _WIDTH + THIS_X] = ledsMem[x][y];
      } else {
        // если нечётная строка
        leds[THIS_Y * _WIDTH + _WIDTH - THIS_X - 1] = ledsMem[x][y];
      }
    }
  }
  FastLED.show();
}

void flushLedsMemFire() {
  for (byte y = 0; y < 16; y++) {
    for (byte x = 0; x < 16; x++) {
      CRGB led = ledsMem[x][y];
      int ledNum = 0;
      if ((THIS_Y % 2 == 0) || MATRIX_TYPE) {
        // если чётная строка
        ledNum = THIS_Y * _WIDTH + THIS_X;
      } else {
        ledNum = THIS_Y * _WIDTH + _WIDTH - THIS_X - 1;
      }
      leds[ledNum].r = led.g;
      leds[ledNum].g = led.b;
      leds[ledNum].b = led.r;
    }
  }
  FastLED.show();
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(int8_t x, int8_t y) {
  if (x < 0 || x > WIDTH - 1) return 0;
  if (y < 0 || y > HEIGHT - 1) return 0;
  return (((uint32_t)ledsMem[x][y].r << 16) | ((long)ledsMem[x][y].g << 8 ) | (long)ledsMem[x][y].b);
}

void blackOut() {
  for (int i =0; i < NUM_LEDS; i++) {
    leds[i] = bgColor;
  }
  for (byte y = 0; y < 16; y++) {
    for (byte x = 0; x < 16; x++) {
      ledsMem[x][y] = bgColor;
    }
  }
  FastLED.show();
}

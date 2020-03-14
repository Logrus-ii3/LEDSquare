//these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
  {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
  {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};

// ******************* НАСТРОЙКИ *****************
// "масштаб" эффектов. Чем меньше, тем крупнее!
#define MADNESS_SCALE 100
#define CLOUD_SCALE 30
#define LAVA_SCALE 50
#define PLASMA_SCALE 30
#define RAINBOW_SCALE 30
#define RAINBOW_S_SCALE 20
#define ZEBRA_SCALE 30
#define FOREST_SCALE 120
#define OCEAN_SCALE 90

class Effects {
  private:
    char* modesJson = "[{\"id\":0,\"name\":\"Конфетти\"},{\"id\":1,\"name\":\"Огонь\"},{\"id\":2,\"name\":\"Вертикальная радуга\"},{\"id\":3,\"name\":\"Горизонтальная радуга\"},{\"id\":4,\"name\":\"Цвета\"},{\"id\":14,\"name\":\"Цвет\"},{\"id\":15,\"name\":\"Снегопад\"},{\"id\":16,\"name\":\"Матрица\"},{\"id\":17,\"name\":\"Светляки\"}]";
    long updateTickMillis;
    long prevTickMillis;
    int currentMode = -1;
    byte scale = 40;
    byte hue;

    // ***************** ДЛЯ РАЗРАБОТЧИКОВ ******************
    
    // The 16 bit version of our coordinates
    static uint16_t x;
    static uint16_t y;
    static uint16_t z;
    
    uint16_t speed = 20; // speed is set dynamically once we've started up
    //uint16_t scale = 30; // scale is set dynamically once we've started up
    
    // This is the array that we keep our computed noise values in
#define MAX_DIMENSION (max(WIDTH, HEIGHT))
#if (WIDTH > HEIGHT)
    uint8_t noise[WIDTH][WIDTH];
#else
    uint8_t noise[HEIGHT][HEIGHT];
#endif
    
    uint8_t colorLoop = 1;
    uint8_t ihue = 0;

    // --------------------------------- конфетти ------------------------------------
    void sparklesRoutine() {
      for (byte i = 0; i < scale; i++) {
        byte x = random(0, WIDTH);
        byte y = random(0, HEIGHT);
        if (getPixColorXY(x, y) == 0)
          ledsMem[x][y] = CHSV(random(0, 255), 255, 255);
      }
      fader(70);
    }
    
    // функция плавного угасания цвета для всех пикселей
    void fader(byte step) {
      for (byte i = 0; i < WIDTH; i++) {
        for (byte j = 0; j < HEIGHT; j++) {
          fadePixel(i, j, step);
        }
      }
    }
    void fadePixel(byte i, byte j, byte step) {     // новый фейдер
      if (getPixColorXY(i, j) == 0) return;
    
      if (ledsMem[i][j].r >= 30 ||
          ledsMem[i][j].g >= 30 ||
          ledsMem[i][j].b >= 30) {
        ledsMem[i][j].fadeToBlackBy(step);
      } else {
        ledsMem[i][j] = 0;
      }
    }
    
    void matrixRoutine() {
      for (byte x = 0; x < WIDTH; x++) {
        // заполняем случайно верхнюю строку
        uint32_t thisColor = getPixColorXY(x, HEIGHT - 1);
        if (thisColor == 0)
          ledsMem[x][HEIGHT-1] = 0x00FF00 * (random(0, scale) == 0);
        else if (thisColor < 0x002000)
          ledsMem[x][HEIGHT-1] = 0;
        else
          ledsMem[x][HEIGHT-1] = thisColor - 0x002000;
      }
    
      // сдвигаем всё вниз
      for (byte x = 0; x < WIDTH; x++) {
        for (byte y = 0; y < HEIGHT - 1; y++) {
          ledsMem[x][y] = getPixColorXY(x, y + 1);
        }
      }
    }

    // -------------------------------------- огонь ---------------------------------------------
    // эффект "огонь"
#define SPARKLES 1        // вылетающие угольки вкл выкл
    unsigned char matrixValue[8][16]; 
    unsigned char line[WIDTH];
    int pcnt = 0;
    
    void fireRoutine() {
      if (pcnt >= 100) {
        shiftUp();
        generateLine();
        pcnt = 0;
      }
      drawFrame(pcnt);
      pcnt += 30;
    }
    
    // Randomly generate the next line (matrix row)
    
    void generateLine() {
      for (uint8_t x = 0; x < WIDTH; x++) {
        line[x] = random(64, 255);
      }
    }
    
    void shiftUp() {
      for (uint8_t y = HEIGHT - 1; y > 0; y--) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          uint8_t newX = x;
          if (x > 15) newX = x - 15;
          if (y > 7) continue;
          matrixValue[y][newX] = matrixValue[y - 1][newX];
        }
      }
    
      for (uint8_t x = 0; x < WIDTH; x++) {
        uint8_t newX = x;
        if (x > 15) newX = x - 15;
        matrixValue[0][newX] = line[newX];
      }
    }
    
    // draw a frame, interpolating between 2 "key frames"
    // @param pcnt percentage of interpolation
    
    void drawFrame(int pcnt) {
      int nextv;
    
      //each row interpolates with the one before it
      for (unsigned char y = HEIGHT - 1; y > 0; y--) {
        for (unsigned char x = 0; x < WIDTH; x++) {
          uint8_t newX = x;
          if (x > 15) newX = x - 15;
          if (y < 8) {
            nextv =
              (((100.0 - pcnt) * matrixValue[y][newX]
                + pcnt * matrixValue[y - 1][newX]) / 100.0)
              - pgm_read_byte(&(valueMask[y][newX]));
    
            CRGB color = CHSV(
                           scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])), // H
                           255, // S
                           (uint8_t)max(0, nextv) // V
                         );
    
            ledsMem[x][y] = color;
          } else if (y == 8 && SPARKLES) {
            if (random(0, 20) == 0 && (int)ledsMem[x][y-1] != 0) ledsMem[x][y] = ledsMem[x][y - 1];
            else ledsMem[x][y] = 0;
          } else if (SPARKLES) {
            // старая версия для яркости
            if ((int)ledsMem[x][y - 1] > 0)
              ledsMem[x][y] = ledsMem[x][y - 1];
            else ledsMem[x][y] = 0;
          }
        }
      }
    
      //first row interpolates with the "next" line
      for (unsigned char x = 0; x < WIDTH; x++) {
        uint8_t newX = x;
        if (x > 15) newX = x - 15;
        CRGB color = CHSV(
                       40 * 2.5 + pgm_read_byte(&(hueMask[0][newX])), // H
                       255,           // S
                       (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
                     );
        ledsMem[newX][0] = color;
      }
    }

    // ---------------------------------------- радуга ------------------------------------------
    void rainbowVertical() {
      hue += 2;
      for (byte j = 0; j < HEIGHT; j++) {
        CHSV thisColor = CHSV((byte)(hue + j * scale), 255, 255);
        for (byte i = 0; i < WIDTH; i++)
          ledsMem[i][j] = thisColor;
      }
    }
    void rainbowHorizontal() {
      hue += 2;
      for (byte i = 0; i < WIDTH; i++) {
        CHSV thisColor = CHSV((byte)(hue + i * scale), 255, 255);
        for (byte j = 0; j < HEIGHT; j++)
          ledsMem[i][j] = thisColor;
      }
    }

    // ---------------------------------------- ЦВЕТА ------------------------------------------
    void colorsRoutine() {
      hue += this->scale;
      for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
          ledsMem[x][y] = CHSV(hue, 255, 255);
        }
      }
    }

    void colorRoutine() {
      for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
          ledsMem[x][y] = CHSV(scale * 2.5, 255, 255);
        }
      }
    }

    // ------------------------------ снегопад 2.0 --------------------------------
    void snowRoutine() {
      // сдвигаем всё вниз
      for (byte x = 0; x < WIDTH; x++) {
        for (byte y = 0; y < HEIGHT - 1; y++) {
          ledsMem[x][y] = getPixColorXY(x, y + 1);
        }
      }

      for (byte x = 0; x < WIDTH; x++) {
        // заполняем случайно верхнюю строку
        // а также не даём двум блокам по вертикали вместе быть
        if (getPixColorXY(x, HEIGHT - 2) == 0 && (random(0, scale) == 0))
          ledsMem[x][HEIGHT - 1] = 0xE0FFFF - 0x101010 * random(0, 4);
        else
          ledsMem[x][HEIGHT - 1] = 0x000000;
      }
    }

    // ----------------------------- СВЕТЛЯКИ ------------------------------
#define LIGHTERS_AM 100
    int lightersPos[2][LIGHTERS_AM];
    int8_t lightersSpeed[2][LIGHTERS_AM];
    CHSV lightersColor[LIGHTERS_AM];
    byte loopCounter;
    
    int angle[LIGHTERS_AM];
    int speedV[LIGHTERS_AM];
    int8_t angleSpeed[LIGHTERS_AM];
    
    void lightersRoutine() {
      FastLED.clear();
      if (++loopCounter > 20) loopCounter = 0;
      for (byte i = 0; i < scale; i++) {
        if (loopCounter == 0) {     // меняем скорость каждые 255 отрисовок
          lightersSpeed[0][i] += random(-3, 4);
          lightersSpeed[1][i] += random(-3, 4);
          lightersSpeed[0][i] = constrain(lightersSpeed[0][i], -20, 20);
          lightersSpeed[1][i] = constrain(lightersSpeed[1][i], -20, 20);
        }
    
        lightersPos[0][i] += lightersSpeed[0][i];
        lightersPos[1][i] += lightersSpeed[1][i];
    
        if (lightersPos[0][i] < 0) lightersPos[0][i] = (WIDTH - 1) * 10;
        if (lightersPos[0][i] >= WIDTH * 10) lightersPos[0][i] = 0;
    
        if (lightersPos[1][i] < 0) {
          lightersPos[1][i] = 0;
          lightersSpeed[1][i] = -lightersSpeed[1][i];
        }
        if (lightersPos[1][i] >= (HEIGHT - 1) * 10) {
          lightersPos[1][i] = (HEIGHT - 1) * 10;
          lightersSpeed[1][i] = -lightersSpeed[1][i];
        }
        ledsMem[lightersPos[0][i] / 10][lightersPos[1][i] / 10] = lightersColor[i];
      }
    }

  public:
    Effects(long updateTickMillis) {
      this->updateTickMillis = updateTickMillis;
      this->prevTickMillis = millis();
    }

    void tick() {
      if (millis() < updateTickMillis + prevTickMillis) {
        return;
      }
      prevTickMillis = millis();
      switch(this->currentMode) {
        case -1: break;
        case 0: sparklesRoutine(); break;
        case 1: fireRoutine(); break;
        case 2: rainbowVertical(); break;
        case 3: rainbowHorizontal(); break;
        case 4: colorsRoutine(); break;
        case 14: colorRoutine(); break;
        case 15: snowRoutine(); break;
        case 16: matrixRoutine(); break;
        case 17: lightersRoutine(); break;
      }

      if (this->currentMode == 1) {
        flushLedsMemFire();
      } else {
        flushLedsMem();
      }
    }

    void setMode(int m) {
      this->currentMode = m;
      if (m == 17) {
        randomSeed(millis());
        for (byte i = 0; i < LIGHTERS_AM; i++) {
          lightersPos[0][i] = random(0, WIDTH * 10);
          lightersPos[1][i] = random(0, HEIGHT * 10);
          lightersSpeed[0][i] = random(-10, 10);
          lightersSpeed[1][i] = random(-10, 10);
          lightersColor[i] = CHSV(random(0, 255), 255, 255);
        }
      }
      blackOut();
    }

    void setTickDelay(long tickDelay) {
      this->updateTickMillis = tickDelay;
    }

    void setScale(byte scale) {
      this->scale = scale;
    }

    String getModes() {
      return modesJson;
    }
};

Effects effects(50);

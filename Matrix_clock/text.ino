class Text {
  private:
    const int topY = 5;
    String text;
    int minX = 0;
    int minY = 5;
    int maxX = 15;
    int maxY = 15;
    long prevTickMillis = 0;
    long tickMillis = 50;
    int curX = maxX;

    uint8_t getIndex(char c) {
      uint8_t ret = c - '0' + 16;
    
      if (ret >= 112 && ret <= 159) {
        ret = ret - 17;
      } else if (ret >= 96 && ret <= 111) {
        ret = ret + 47;
      }
      return ret;
    }
    
    void drawLetter(char letter, int x, int y) {
      if (x > maxX || y > maxY || x + 6 < minX) {
        return;
      }

      uint8_t letterIndex = getIndex(letter);

      for (byte col = 0; col < 5; col++) {
        if (col+x > maxX) {
          return;
        }
        if (col+x < minX) {
          continue;
        }
        uint8_t byteCol = pgm_read_byte(&(fontHEX[letterIndex][col]));
        for (byte i = 0; i < 8; i++) {
          if (i+y > maxY) {
            continue;
          }
          boolean showBit = byteCol & (1 << i);
          if (showBit) {
            ledsMem[col+x][i+y] = CRGB(3, 3, 3);
          } else {
            ledsMem[col+x][i+y] = bgColor;
          }
        }
      }
      if ((x+5 <= maxX) && (x+5 >= minX)) {
        for (byte i = 0; i < 8; i++) {
          ledsMem[x+5][i+y] = bgColor;
        }
      }
    }

  public:
    Text(long tickMillis) {
      this->tickMillis = tickMillis;
      this->text = (char *)0;
      this->text.reserve(255);
    }

    void setText(String text) {
      const char* t = text.c_str();
      this->text = (char *)0;
      for (int i = 0; t[i] != 0; i++) {
        if (t[i] != 208 && t[i] != 209) {
          this->text += t[i];
        }
      }

      curX = maxX;
      this->clear();
    }

    void tick() {
      if (millis() < tickMillis + prevTickMillis) {
        return;
      }
      prevTickMillis = millis();

      curX = curX - 1;
      int limit = 0 - text.length() * 6 + minX;
      if (curX < limit) {
        curX = maxX;
      }
      for (uint8_t i = 0; i < text.length(); i++) {
        drawLetter(text[i], curX + i*6, topY);
      }
      flushLedsMem();
    }

    void clear() {
      for (int i = minX; i <= maxX; i++) {
        for (int j = minY; j <= maxY; j++) {
          ledsMem[i][j] = bgColor;
        }
      }
      flushLedsMem();
    }

};

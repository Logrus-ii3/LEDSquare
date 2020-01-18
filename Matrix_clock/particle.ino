class Tail {
  public:
    byte x;
    byte y;
    CRGB color;
    Tail(){}
    Tail(byte x, byte y, CRGB color) {
      this->x = x;
      this->y = y;
      this->color = color;
    }
};

class Particle {
  private:
    int stepDelay;
    CRGB color;
    long particleTime = 0;
    byte tailLen = 1;
    byte minX = 0;
    byte minY = 5;
    byte maxX = 15;
    byte maxY = 15;
    byte x = random(minX, maxX+1);
    byte y = random(minY, maxY+1);
    int vX = -1;
    int vY = -1;
    Tail tail[11];

  public:
    Particle(CRGB color, CRGB bgColor, int stepDelay, byte len) {
      this->color = color;
      this->stepDelay = stepDelay;
      this->tailLen = len + 1;
      if (this->tailLen > 10) {
        this->tailLen = 10;
      }
      for (byte i = 0; i < len ; i++) {
        tail[i] = Tail(this->x, this->y, this->color);
      }
      tail[len+1] = Tail(this->x, this->y, bgColor);
    }

    void tick() {
      if (millis() < this->particleTime + this->stepDelay) {
        return;
      }
      this->particleTime = millis();
      this->stepCoordinates();
      this->draw();
    }
  private:
    void stepCoordinates() {
      for (byte i = tailLen - 1; i > 0; i--) {
        tail[i].x = tail[i-1].x;
        tail[i].y = tail[i-1].y;
      }
      x = x + vX;
      if (x < minX || x > maxX) {
        x = x - vX;
        vX = -vX;
        x = x + vX;
      }
      y = y + vY;
      if (y < minY || y > maxY) {
        y = y - vY;
        vY = -vY;
        y = y + vY;
      }
      tail[0].x = x;
      tail[0].y = y;
    }

    void draw() {
      for (byte i = tailLen; i > 0; i--) {
        ledsMem[tail[i].x][tail[i].y] = tail[i].color;
      }
      ledsMem[tail[0].x][tail[0].y] = tail[0].color;
      flushLedsMem();
    }
};

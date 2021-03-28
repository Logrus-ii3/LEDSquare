// https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectRainbowCycle
class Rainbow {
  private:
    long lastTimestamp = 0;
    int stepDelay = 50;
    bool isTicking = false;
    int currentStep = 0;

    void nextStep() {
      byte *c;
      this->currentStep++;
      if (this->currentStep > 256*5) {
        this->currentStep = 0;
      }
      for (int i = 0; i < NUM_LEDS; i++) {
        c = Wheel(((i * 256 / NUM_LEDS) + this->currentStep) & 255);
        leds[i][0] = *c;
        leds[i][1] = *(c+1);
        leds[i][2] = *(c+2);
      }
      showLeds();
    }

    byte * Wheel(byte WheelPos) {
      static byte c[3];
     
      if(WheelPos < 85) {
       c[0]=WheelPos * 3;
       c[1]=255 - WheelPos * 3;
       c[2]=0;
      } else if(WheelPos < 170) {
       WheelPos -= 85;
       c[0]=255 - WheelPos * 3;
       c[1]=0;
       c[2]=WheelPos * 3;
      } else {
       WheelPos -= 170;
       c[0]=0;
       c[1]=WheelPos * 3;
       c[2]=255 - WheelPos * 3;
      }
    
      return c;
    }

  public:
    Rainbow(int stepDelay) {
      this->stepDelay = stepDelay;
    }

    void start() {
      this->isTicking = true;
    }

    void stop() {
      this->isTicking = false;
    }

    void setStepDelay(int stepDelay) {
      if (stepDelay < 0) {
        this->stepDelay = 0;
      } else {
        this->stepDelay = stepDelay;
      }
    }

    int getStepDelay() {
      return this->stepDelay;
    }

    bool ticking() {
      return this->isTicking;
    }

    void tick() {
      if (!this->isTicking) {
        return;
      }
      if (millis() < this->lastTimestamp + this->stepDelay) {
        return;
      }
      this->lastTimestamp = millis();
      this->nextStep();
    }
};

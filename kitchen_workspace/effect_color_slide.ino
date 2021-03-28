class ColorSlide {
  private:
    long lastTimestamp = 0;
    int stepDelay = 50;
    bool isTicking = false;
    int currentStep = 0;
    int red, green, blue;

    void nextStep() {
      leds[this->currentStep][0] = this->red;
      leds[this->currentStep][1] = this->green;
      leds[this->currentStep][2] = this->blue;
      leds[NUM_LEDS-this->currentStep-1][0] = this->red;
      leds[NUM_LEDS-this->currentStep-1][1] = this->green;
      leds[NUM_LEDS-this->currentStep-1][2] = this->blue;
      showLeds();
      this->currentStep++;
      if (this->currentStep > (NUM_LEDS) / 2 + 1) {
        this->stop();
      }
    }

  public:
    ColorSlide(int stepDelay) {
      this->stepDelay = stepDelay;
    }

    void start(int red, int green, int blue) {
      this->isTicking = true;
      this->red = red;
      this->green = green;
      this->blue = blue;
      this->currentStep = 0;
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

class Girland {
  private:
    long lastTimestamp = 0;
    int stepDelay = 50;
    bool isTicking = false;
    bool fade = false;
    int stepsCount = 100;
    int currentColor = 0;
    int currentColorStep = 0;
    int colors[GIRLAND_COLORS_COUNT][3] = {
      { 255, 0, 0 },
      { 255, 255, 0 },
      { 0, 255, 0 },
      { 0, 255, 255 },
      { 0, 0, 255 },
      { 255, 0, 255 }
    };
    int colorsSteps[GIRLAND_COLORS_COUNT][3];

    void nextStep() {
      this->currentColorStep++;
      if (this->currentColorStep > this->stepsCount) {
        this->changeColors();
      }
      for (int i = 0; i < NUM_LEDS; i++) {
        int colorIndex = i % GIRLAND_COLORS_COUNT;
        for (int j = 0; j < 3; j++) {
          if (colorIndex == this->currentColor) {
            if (fade) {
              leds[i][j] = leds[i][j] - this->colorsSteps[colorIndex][j];
              if (leds[i][j] < 0) {
                leds[i][j] = 0;
              }
            } else {
              leds[i][j] = leds[i][j] + this->colorsSteps[colorIndex][j];
              if (leds[i][j] > maxBrightness) {
                leds[i][j] = maxBrightness;
              }
            }
          }
        }
      }
      showLeds();
    }

    void changeColors() {
      this->currentColorStep = 0;
      if (this->fade) {
        this->currentColor++;
        if (this->currentColor > GIRLAND_COLORS_COUNT-1) {
          this->currentColor = 0;
        }
      }
      this->fade = !this->fade;
    }
  
  public:
    Girland(int stepDelay) {
      this->stepDelay = stepDelay;
    }

    void start() {
      this->isTicking = true;
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i][0] = colors[NUM_LEDS%GIRLAND_COLORS_COUNT][0];
        leds[i][1] = colors[NUM_LEDS%GIRLAND_COLORS_COUNT][1];
        leds[i][2] = colors[NUM_LEDS%GIRLAND_COLORS_COUNT][2];
      }
      for (int i = 0; i < GIRLAND_COLORS_COUNT; i++) {
        for (int j = 0; j < 3; j++) {
          this->colorsSteps[i][j] = this->colors[i][j] / this->stepsCount;
        }
      }
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

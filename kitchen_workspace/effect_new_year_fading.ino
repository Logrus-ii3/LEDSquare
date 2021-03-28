class NewYearFading {
  private:
    long lastTimestamp = 0;
    int stepDelay = 50;
    int currentColor = 0;
    int currentColorStep = 0;
    bool isTicking = false;
    int stepsCount = 100;
    int previousColors[3] = {0, 0, 0};
    int colorsSteps[3] = {0, 0, 0};
    int colors[6][3] = {
      { 255, 0, 0 },
      { 255, 255, 0 },
      { 0, 255, 0 },
      { 0, 255, 255 },
      { 0, 0, 255 },
      { 255, 0, 255 }
      };
    int colorsCount = sizeof(colors) / sizeof(colors[0]);

    void nextStep() {
      this->currentColorStep++;
      if (this->currentColorStep > this->stepsCount) {
        this->changeColors();
      }
      for (int i = 0; i < NUM_LEDS; i++) {
        for (int j = 0; j < 3; j++) {
          leds[i][j] = leds[i][j] + this->colorsSteps[j];
          if (leds[i][j] < 0) {
            leds[i][j] = 0;
          }
          if (leds[i][j] > maxBrightness) {
            leds[i][j] = maxBrightness;
          }
        }
      }

      showLeds();
    }

    void changeColors() {
      int previousColorIndex = this->currentColor;
      for (int i = 0; i < 3; i++) {
        this->previousColors[i] = this->colors[this->currentColor][i];
      }
      this->currentColorStep = 0;
      this->currentColor++;
      if (this->currentColor > this->colorsCount-1) {
        this->currentColor = 0;
      }

      for (int i = 0; i < 3; i++) {
        this->colorsSteps[i] = (this->colors[this->currentColor][i] - this->colors[previousColorIndex][i]) / this->stepsCount;
      }
    }

  public:
    NewYearFading(int stepDelay) {
      this->stepDelay = stepDelay;
    }

    void start() {
      this->isTicking = true;
      this->currentColorStep = 0;
      this->currentColor = 0;
      for (int i = 0; i < 3; i++) {
        this->colorsSteps[i] = this->colors[0][i] / this->stepsCount;
        this->previousColors[i] = 0;
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

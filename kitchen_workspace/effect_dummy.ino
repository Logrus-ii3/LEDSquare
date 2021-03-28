class Dummy {
  private:
    long lastTimestamp = 0;
    int stepDelay = 50;
    bool isTicking = false;
    int currentStep = 0;

    void nextStep() {
    }

  public:
    Dummy(int stepDelay) {
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

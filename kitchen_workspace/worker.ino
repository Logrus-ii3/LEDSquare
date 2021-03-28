class Worker {
  private:
    bool isTicking = false;
    Girland girland = Girland(50);
    NewYearFading newYearFading = NewYearFading(50);
    Rainbow rainbow = Rainbow(20);
    ColorSlide colorSlide = ColorSlide(10);
    int effect = 0;

  public:
    Worker() {
    }

    void runColor(int red, int green, int blue) {
      this->stop();
      this->colorSlide.start(red, green, blue);
      this->effect = 3;
      this->start();
    }

    void setEffect(int effect) {
      Serial.printf("Worker:setEffect %d\n", effect);
      this->stop();
      this->effect = effect;
      this->start();
    }

    int getEffect() {
      return this->effect;
    }

    void start() {
      Serial.println("Worker:start");
      this->isTicking = true;
      switch (this->effect) {
        case 0: this->newYearFading.start(); break;
        case 1: this->girland.start(); break;
        case 2: this->rainbow.start(); break;
        case 3: break; // No need in starting it here.
      }
    }

    void stop() {
      Serial.println("Worker:stop");
      this->isTicking = false;
      this->newYearFading.stop();
      this->girland.stop();
      this->rainbow.stop();
      this->colorSlide.stop();
    }

    void setStepDelay(int stepDelay) {
      switch (this->effect) {
        case 0: this->newYearFading.setStepDelay(stepDelay); break;
        case 1: this->girland.setStepDelay(stepDelay); break;
        case 2: this->rainbow.setStepDelay(stepDelay); break;
        case 3: this->colorSlide.setStepDelay(stepDelay); break;
      }
    }

    int getStepDelay() {
      int stepDelay = -1;
      switch (this->effect) {
        case 0: stepDelay = this->newYearFading.getStepDelay(); break;
        case 1: stepDelay = this->girland.getStepDelay(); break;
        case 2: stepDelay = this->rainbow.getStepDelay(); break;
        case 3: stepDelay = this->colorSlide.getStepDelay(); break;
      }
      return stepDelay;
    }

    bool ticking() {
      return this->isTicking;
    }

    void tick() {
      if (!this->isTicking) {
        return;
      }
      switch (this->effect) {
        case 0: this->newYearFading.tick(); break;
        case 1: this->girland.tick(); break;
        case 2: this->rainbow.tick(); break;
        case 3:
          this->colorSlide.tick();
          if (!this->colorSlide.ticking()) {
            this->stop();
          }
          break;
      }
    }
};
Worker worker;

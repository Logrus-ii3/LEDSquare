class Timer {
  unsigned long millisStart = 0;
  unsigned long millisToOff = 0;
  boolean ticking = false;

  public:
    Timer(void) {
    }

    void start(unsigned long millisToOff) {
      this->millisStart = millis();
      this->millisToOff = millisToOff;
      this->ticking = true;
      Serial.print("Timer started with ");
      Serial.println(millisToOff);
    }

    int doStep(void) {
      if (this->ticking) {
        if (millis() - this->millisStart > this->millisToOff) {
          this->finish();
          return 1;
        }
      }
      return 0;
    }

    void finish() {
      this->ticking = false;
      Serial.println("Timer stopped.");
    }

};

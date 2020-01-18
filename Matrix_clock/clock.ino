bool tick = false;
long clockTickMillis = 500;
CRGB clockColor = CRGB(5, 0, 0);
CRGB tickTockColor = CRGB(5, 5, 0);
long clockPrevMillis = 0;

void clockTick() {
  if (millis() > clockPrevMillis + clockTickMillis) {
    clockPrevMillis = millis();
    printClock(0);
    flushLedsMem();
  }
}

void printClock(byte Y) {
  if (tick) {
    ledsMem[7][Y+2] = tickTockColor;
    ledsMem[8][Y+2] = bgColor;
    ledsMem[7][Y+3] = bgColor;
    ledsMem[8][Y+3] = tickTockColor;
  } else {
    ledsMem[7][Y+2] = bgColor;
    ledsMem[8][Y+2] = tickTockColor;
    ledsMem[7][Y+3] = tickTockColor;
    ledsMem[8][Y+3] = bgColor;
  }
  tick = !tick;
  
  // hour
  printDoubleDigits(hour(), 0, Y, clockColor, bgColor);
  // minute
  printDoubleDigits(minute(), 9, Y, clockColor, bgColor);
}

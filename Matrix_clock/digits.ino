const uint8_t font3x5_old[][3] PROGMEM = {
  {0b11111, 0b10001, 0b11111},    // 0
  {0b00000, 0b00000, 0b11111},    // 1
  {0b10111, 0b10101, 0b11101},    // 2
  {0b10101, 0b10101, 0b11111},    // 3
  {0b11100, 0b00100, 0b11111},    // 4
  {0b11101, 0b10101, 0b10111},    // 5
  {0b11111, 0b10101, 0b10111},    // 6
  {0b10000, 0b10000, 0b11111},    // 7
  {0b11111, 0b10101, 0b11111},    // 8
  {0b11101, 0b10101, 0b11111},    // 9
};

const uint8_t font3x5[][3] PROGMEM = {
  {0b11111, 0b10001, 0b11111},    // 0
  {0b00000, 0b00000, 0b11111},    // 1
  {0b10111, 0b10101, 0b11101},    // 2
  {0b10101, 0b10101, 0b11111},    // 3
  {0b11100, 0b00100, 0b11111},    // 4
  {0b11101, 0b10101, 0b10111},    // 5
  {0b11111, 0b10101, 0b10111},    // 6
  {0b10000, 0b10000, 0b11111},    // 7
  {0b11111, 0b10101, 0b11111},    // 8
  {0b11101, 0b10101, 0b11111},    // 9
};

void printDigit(byte digit, byte X, byte Y, CRGB color, CRGB bgColor) {
  for (byte x = 0; x < 3; x++) {
    byte digitDefinition = pgm_read_byte(&(font3x5[digit][x]));
    for (byte y = 0; y < 5; y++) {
      if (digitDefinition & (1 << 4 - y)) {
        ledsMem[X+x][Y+y] = color;
      } else {
        ledsMem[X+x][Y+y] = bgColor;
      }
    }
  }
}

void printDoubleDigits(byte digits, byte X, byte Y, CRGB color, CRGB bgColor) {
  byte firstDigit = 0;
  if (digits > 9) {
    firstDigit = digits / 10;
  }
  byte secondDigit = digits % 10;
  printDigit(firstDigit, X, Y, color, bgColor);
  printDigit(secondDigit, X+4, Y, color, bgColor);
}

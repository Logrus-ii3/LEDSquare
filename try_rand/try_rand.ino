#include <time.h>
void setup() {
  delay(1000);
  Serial.begin(115200);
  srand( (unsigned)time(NULL) );
}

void loop() {
  Serial.println(rand() % 5);
  delay(1000);
}

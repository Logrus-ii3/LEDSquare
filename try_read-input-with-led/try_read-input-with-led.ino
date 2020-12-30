const int inputPin = 5;
int pressed = 0;
unsigned long lastRead = 0;
unsigned long filterDelay = 50;

void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  pinMode(inputPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pressed = digitalRead(inputPin);
  Serial.print("Begin with ");
  Serial.println(pressed);
}

void loop() {
  int currentState = digitalRead(inputPin);
  if (currentState != pressed) {
    lastRead = millis();
    Serial.print(currentState);
  }

  if ((millis() - lastRead) > filterDelay) {
    if (currentState != pressed) {
      pressed = currentState;
      Serial.print("### ");
      Serial.print(pressed);
      Serial.println(" ###");
    }
  }
}

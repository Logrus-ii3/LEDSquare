#include "config.h"

// LED parameters
const int redPin = D5;
const int greenPin = D6;
const int bluePin = D7;
const int maxAllowedBrightness = 512;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

// set up the 'bedroom-circle' feed
AdafruitIO_Feed *circle = io.feed("bedroom-circle");

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

    void doStep(void) {
      if (this->ticking) {
        if (millis() - this->millisStart > this->millisToOff) {
          this->doStop();
        }
      }
    }

    void doStop(void) {
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      this->ticking = false;
      Serial.println("Timer stopped.");
    }
};

class Dimmer {
  int maxSteps;
  int duration;
  int stepDuration;
  int currentStep = 0;
  unsigned long lastTimestamp = 0;
  boolean goingUp = true;
  boolean dimming = false;
  int maxRed;
  int maxGreen;
  int maxBlue;

  void doStepUp(void) {
    this->setStepBrightness();
    this->currentStep++;
    if (this->currentStep == this->maxSteps) {
      this->goingUp = false;
    }
  }
  
  void doStepDown(void) {
    this->setStepBrightness();
    this->currentStep--;
    if (this->currentStep == 0) {
      this->goingUp = true;
    }
  }

  void setStepBrightness(void) {
    int red = map(this->currentStep, 0, this->maxSteps, 0, this->maxRed);
    int green = map(this->currentStep, 0, this->maxSteps, 0, this->maxGreen);
    int blue = map(this->currentStep, 0, this->maxSteps, 0, this->maxBlue);
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  }
  
  public:
    Dimmer(int maxSteps, int duration, int maxRed, int maxGreen, int maxBlue) {
      this->set(maxSteps, duration, maxRed, maxGreen, maxBlue);
    }

    void set(int maxSteps, int duration, int maxRed, int maxGreen, int maxBlue) {
      this->doStop();
      this->maxSteps = maxSteps;
      this->duration = duration;
      this->stepDuration = duration / maxSteps;
      this->setColor(maxRed, maxGreen, maxBlue);
    }

    void setColor(int maxRed, int maxGreen, int maxBlue) {
      this->doStop();
      this->maxRed = maxRed;
      this->maxGreen = maxGreen;
      this->maxBlue = maxBlue;
    }

    void setColorAndStart(int maxRed, int maxGreen, int maxBlue) {
      this->setColor(maxRed, maxGreen, maxBlue);
      this->start();
    }

    void start(void) {
      this->lastTimestamp = millis();
      this->dimming = true;
      this->currentStep = 0;
      this->goingUp = true;
    }
    
    void doStop() {
      this->dimming = false;
    }

    // Main method to do things
    void doStep(void) {
      if (!this->dimming) {
        return;
      }
      if (this->lastTimestamp + this->stepDuration < millis()) {
        // We need to step
        if (this->goingUp) {
          this->doStepUp();
        } else {
          this->doStepDown();
        }
        this->lastTimestamp = millis();
      }
    }

    void increaseDuration(void) {
      this->doStop();
      if (this->duration < 30500) {
        this->duration += 1000;
      }
      this->stepDuration = this->duration / this->maxSteps;
      this->start();
    }

    void decreaseDuration(void) {
      this->doStop();
      if (this->duration > 1500) {
        this->duration -= 1000;
      }
      this->stepDuration = this->duration / this->maxSteps;
      this->start();
    }
    
    boolean isDimming(void) {
      return this->dimming;
    }
};

Dimmer dimmer = Dimmer(100, 5000, 512, 512, 512);
Timer timer = Timer();

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setBrightness(0);
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  circle->onMessage(handleMessage);
  circle->get();
}

void loop() {
  io.run();
  dimmer.doStep();
  timer.doStep();
}

void setBrightness(int red, int green, int blue) {
  dimmer.doStop();
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
  redValue = red;
  greenValue = green;
  blueValue= blue;
}

// Sets white brightness
void setBrightness(int value) {
  setBrightness(value, value, value);
}

void handleMessage(AdafruitIO_Data *data) {

  String message = data->value();

  Serial.print("received <- ");
  Serial.println(message);

  message.toLowerCase();

  if (message.startsWith("rgb ")) {
    setRGB(message);
  }
  
  // Colors
  else if (message == "on" || message == "white") {
    setBrightness(maxAllowedBrightness);
  } else if (message == "off") {
    setBrightness(0);
  } else if (message == "red") {
    setBrightness(maxAllowedBrightness, 0, 0);
  } else if (message == "green") {
    setBrightness(0, maxAllowedBrightness, 0);
  } else if (message == "blue") {
    setBrightness(0, 0, maxAllowedBrightness);
  } else if (message == "purple") {
    setBrightness(maxAllowedBrightness, 0, maxAllowedBrightness);
  } else if (message == "pink") {
    setBrightness(maxAllowedBrightness, 70, 162);
  } else if (message == "yellow") {
    setBrightness(maxAllowedBrightness, 173*4, 40);
  } else if (message == "maximum white") {
    setBrightness(1023, 1023, 1023);
  } else if (message == "night") {
    setBrightness(50, 50, 10);

  // Pulsing colors
  } else if (message == "pulsing" || message == "pulsing white") {
    dimmer.setColorAndStart(maxAllowedBrightness, maxAllowedBrightness, maxAllowedBrightness);
  } else if (message == "pulsing red") {
    dimmer.setColorAndStart(maxAllowedBrightness, 0, 0);
  } else if (message == "pulsing green") {
    dimmer.setColorAndStart(0, maxAllowedBrightness, 0);
  } else if (message == "pulsing blue") {
    dimmer.setColorAndStart(0, 0, maxAllowedBrightness);
  } else if (message == "pulsing purple") {
    dimmer.setColorAndStart(maxAllowedBrightness, 0, maxAllowedBrightness);
  } else if (message == "pulsing pink") {
    dimmer.setColorAndStart(maxAllowedBrightness, 70, 162);
  } else if (message == "pulsing yellow") {
    dimmer.setColorAndStart(maxAllowedBrightness, 173*4, 40);

  // Dimming speed
  } else if (message == "faster") {
    dimmer.decreaseDuration();
  } else if (message == "slower") {
    dimmer.increaseDuration();

  // Timer
  } else if (message.startsWith("timer ")) {
    unsigned int secondsToOff = message.substring(6).toInt();
    timer.start(secondsToOff * 1000);
  }

}

void setRGB(String message) {
  String rgb = message.substring(4);
  rgb.trim();
  int firstSpace = rgb.indexOf(" ");
  int secondSpace = rgb.indexOf(" ", firstSpace + 1);
  int red = rgb.substring(0, firstSpace).toInt();
  int green = rgb.substring(firstSpace + 1, secondSpace).toInt();
  int blue = rgb.substring(secondSpace + 1).toInt();
  Serial.print("R: ");
  Serial.print(red);
  Serial.print(". G: ");
  Serial.print(green);
  Serial.print(". B: ");
  Serial.println(blue);
  setBrightness(red, green, blue);
}

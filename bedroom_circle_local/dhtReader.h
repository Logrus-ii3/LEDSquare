class DhtReader {
  private:
    unsigned long lastRead = 0;
    unsigned long interval;
    int temperature = -100;
    int humidity = -100;
    int tempRead = 0;
    int humRead = 0;

    void doRead() {
      Serial.println("doRead()");
      this->lastRead = millis();
      int reading = (int)dht.readTemperature();
      if (reading != 2147483647) {
        if (this->temperature != reading) {
          this->temperature = reading;
          Serial.print("Publishing temp: ");
          Serial.println(itoa(this->temperature, messageBuffer, 10));
          if(!client.publish(MQTT_TEMPERATURE_TOPIC, itoa(this->temperature, messageBuffer, 10), true)) {
            Serial.print("Publish failed: ");
            Serial.println(client.state());
          }
        }
      } else {
        Serial.println("Could not read temperature");
      }

      reading = (int)dht.readHumidity();
      if (reading != 2147483647) {
        if (this->humidity != reading) {
          this->humidity = reading;
          Serial.print("Publishing humidity: ");
          Serial.println(itoa(this->humidity, messageBuffer, 10));
          if (!client.publish(MQTT_HUMIDITY_TOPIC, itoa(this->humidity, messageBuffer, 10), true)) {
            Serial.print("Publish failed: ");
            Serial.println(client.state());
          }
        }
      } else {
        Serial.println("Could not read humidity");
      }
    }

  public:
    DhtReader(long interval) {
      this->interval = interval;
    }

    void loop(void) {
      if (millis() - this->lastRead > this->interval) {
        this->doRead();
      }
    }

    void setInterval(unsigned long interval) {
      this->interval = interval;
    }

};

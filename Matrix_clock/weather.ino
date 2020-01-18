class Weather {
  private:
    const char* host = "api.openweathermap.org";
    const char* apiPath = "/data/2.5/weather?id=501175&APPID=be954e87e460537012e24933d9a67d9c&lang=ru&units=metric";
    long dt;
    float currentTemperature;
    long updateTickMillis;
    long prevTickMillis;
    long retryTimeoutMillis = 10000;
    Text text = Text(50);

    void displayTemp() {
      
    }

  public:
    Weather(long updateTickMillis) {
      this->updateTickMillis = updateTickMillis;
      this->prevTickMillis = millis();
    }

    void tick() {
      if (millis() > updateTickMillis + prevTickMillis) {
        prevTickMillis = millis();
        this->doUpdate();
      }
      text.tick();
    }

    void doUpdate() {
      Serial.print("Updating weather: ");
      WiFiClient client;
      if (!client.connect(host, 80)) {
        Serial.println("Could not connect to the weather server.");
        client.stop();
        return;
      }
      client.printf("GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", apiPath, host);

      // Check HTTP status
      String status = client.readStringUntil('\r');
      Serial.print(" Answer: ");
      Serial.print(status);
      if (status != "HTTP/1.1 200 OK") {
        Serial.print("Unexpected response: ");
        prevTickMillis = prevTickMillis - updateTickMillis + retryTimeoutMillis;
        return;
      }

      // Skip HTTP headers
      char endOfHeaders[] = "\r\n\r\n";
      if (!client.find(endOfHeaders)) {
        Serial.println("Invalid response");
        return;
      }
      
      // Allocate the JSON document
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, client);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      client.stop();

      //const char* t = doc["name"].as<char*>();
      //text.setText(t);
      Serial.println("Success");

      int wSize = doc["weather"].as<JsonArray>().size();
      String weatherText = "Погода: ";
      weatherText += doc["weather"][0]["description"].as<String>();
      for (int i = 1; i < wSize; i++) {
        weatherText += "/";
        weatherText += doc["weather"][i]["description"].as<String>();
      }
      weatherText += ". Температура: ";
      weatherText += doc["main"]["temp"].as<String>();
      weatherText += "'C Влажность: ";
      weatherText += doc["main"]["humidity"].as<String>();
      weatherText += "%";
      text.setText(weatherText);
      Serial.println(weatherText);
    }
};

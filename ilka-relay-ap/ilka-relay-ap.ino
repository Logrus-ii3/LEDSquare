#include <ESP8266WiFi.h> // Подключение WiFi-библиотеки

int port = 8888;  // Номер TCP-порта для ожидания клиента
const int relayPin = D2; // Пин, к которому подключено реле

IPAddress IP = IPAddress(10, 10, 1, 1);
IPAddress NMask = IPAddress(255, 255, 255, 0);

WiFiServer server(port); // Инициализация сервера
WiFiClient client; // Объявление переменной для клиента

void setup() { // Выполнется один раз при включении
  Serial.begin(115200); // Подключиться к консоли
  delay(1000);
  Serial.println("Turning on WiFi...");
  WiFi.mode(WIFI_AP); // Режим Station для WiFi
  WiFi.softAP("GrinenkoRelay"); // Инициировать подключение к WiFi
  delay(1000);
  Serial.println("Configuring WiFi...");
  WiFi.softAPConfig(IP, IP, NMask);
  delay(1000);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  pinMode(relayPin, OUTPUT); // Установка режима "чтения" для пина реле
  digitalWrite(relayPin, HIGH); // "Включить" реле
  server.begin(); // Запустить сервер, ожидать подключений
  Serial.println("Ready to serve.");
}

void loop() { // Бесконечный цикл, пока работает устройство
  if (!client.connected()) { // Если клиент не подключён
    client = server.available(); // Проверить подключение клиента
  } else { // Если клиент подключён
    if (client.available() > 0) { // Если от клиента пришли данные
      char msgChar = client.read(); // Прочитать один байт от клиента
      Serial.print("Got from client: ");
      Serial.println(msgChar);
      if (msgChar == '0') {
        digitalWrite(relayPin, HIGH); // "Выключить" реле
      } else if (msgChar == '1') {
        digitalWrite(relayPin, LOW); // "Включить" реле
      }
      client.stop(); // Отключить клиента
    }
  }
}

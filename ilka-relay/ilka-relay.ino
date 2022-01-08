#include <ESP8266WiFi.h> // Подключение WiFi-библиотеки
#define WIFI_SSID "MY WIFI"
#define WIFI_PASS "SECRET"

int port = 8888;  // Номер TCP-порта для ожидания клиента
const int relayPin = D2; // Пин, к которому подключено реле

WiFiServer server(port); // Инициализация сервера
WiFiClient client; // Объявление переменной для клиента

void setup() { // Выполнется один раз при включении
  Serial.begin(115200); // Подключиться к консоли
  WiFi.mode(WIFI_STA); // Режим Station для WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS); // Инициировать подключение к WiFi
  while (WiFi.status() != WL_CONNECTED) { // Ожидаем подключения
    Serial.print(".");
    delay(250);
  }
  Serial.print("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP()); // Вывести в консоль полученный IP-адрес
  pinMode(relayPin, OUTPUT); // Установка режима "чтения" для пина реле
  digitalWrite(relayPin, LOW); // "Включить" реле
  server.begin(); // Запустить сервер, ожидать подключений
}

void loop() { // Бесконечный цикл, пока работает устройство
  if (!client.connected()) { // Если клиент не подключён
    client = server.available(); // Проверить подключение клиента
  } else { // Если клиент подключён
    if (client.available() > 0) { // Если от клиента пришли данные
      char msgChar = client.read(); // Прочитать один байт от клиента
      if (msgChar == '0') {
        digitalWrite(relayPin, HIGH); // "Выключить" реле
      } else if (msgChar == '1') {
        digitalWrite(relayPin, LOW); // "Включить" реле
      }
      client.stop(); // Отключить клиента
    }
  }
}

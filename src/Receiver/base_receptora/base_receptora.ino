#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include "time.h"

// ======== CONFIG WIFI =========
#define WIFI_SSID "bruno"
#define WIFI_PASSWORD "bruno1512"

// ======== CONFIG FIREBASE =========
#define API_KEY "AIzaSyBZelS0j53PZxSHDf9LQ3MIqPREXy8t-jg"
#define DATABASE_URL "https://cloudia-22e4d-default-rtdb.firebaseio.com/"
#define USER_EMAIL "cloudia.firebase@gmail.com"
#define USER_PASSWORD "Choveu22mm"

// ======== OBJETOS FIREBASE =========
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ======== OBJETOS LORA =========
HardwareSerial LoRaSerial(2);  // UART2 RX=16, TX=17
JsonDocument doc;

// ======== PINOS LORA =========
#define PIN_M0 21
#define PIN_M1 19
#define PIN_AUX 15

void setLoRaMode(int m0, int m1) {
  digitalWrite(PIN_M0, m0);
  digitalWrite(PIN_M1, m1);
  delay(100);
  while (digitalRead(PIN_AUX) == LOW); // Espera LoRa pronto
  delay(2);
}

// ======== OBJETOS TEMPO =========
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800; // UTC-3 (Brasil)
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  // ====== Inicializa pinos LoRa ======
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_AUX, INPUT);

  // Inicializa UART2 para o LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  setLoRaMode(LOW, LOW);  // Modo 0 - Transparente

  Serial.println("Iniciando LoRa em modo transparente...");

  // ========== Wi-Fi ==========
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  // ========== NTP ==========
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sincronizando horário via NTP...");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter hora");
  } else {
    Serial.println("Hora sincronizada");
  }

  // ========== Firebase ==========
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Esperar Firebase estar pronto
  Serial.println("Conectando ao Firebase...");
  unsigned long timeout = millis() + 10000;
  while (!Firebase.ready()) {
    if (millis() > timeout) {
      Serial.println("Falha ao conectar com Firebase.");
      break;
    }
    delay(100);
  }

  if (Firebase.ready()) {
    Serial.println("Firebase pronto!");
  }
}

void loop() {

  if (LoRaSerial.available()) {
    String input = LoRaSerial.readStringUntil('\n');
    input.trim();

    Serial.print("Recebido via LoRa: ");
    Serial.println(input);

    DeserializationError error = deserializeJson(doc, input);
    if (error) {
      Serial.print("Erro ao decodificar JSON: ");
      Serial.println(error.c_str());
      return;
    }

    // Obtem hora atual
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Erro ao obter data/hora");
      return;
    }

    char datePath[20];
    char timePath[20];
    strftime(datePath, sizeof(datePath), "%Y-%m-%d", &timeinfo);
    strftime(timePath, sizeof(timePath), "%H-%M-%S", &timeinfo);

    String fullPath = "/estacao/" + String(datePath) + "/" + String(timePath);

    // Envia JSON diretamente para o caminho
    FirebaseJson firebaseJson;
    String jsonString;
    serializeJson(doc, jsonString);
    firebaseJson.setJsonData(jsonString);

    if (Firebase.RTDB.setJSON(&fbdo, fullPath.c_str(), &firebaseJson)) {
      Serial.print("Dados enviados para ");
      Serial.println(fullPath);
    } else {
      Serial.print("Erro Firebase: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
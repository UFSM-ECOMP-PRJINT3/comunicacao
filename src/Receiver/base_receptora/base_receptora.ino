#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "AP 201_2G"
#define WIFI_PASSWORD "4A94DF81"

#define API_KEY "AIzaSyCmG3MRoyZF3QAYNOxqGjIlBefPbWIwd9M"
#define DATABASE_URL "https://cloudia-6117c-default-rtdb.firebaseio.com/" 

#define USER_EMAIL ""
#define USER_PASSWORD "" // use o auth padrão ou deixe em branco para modo anônimo

// Objeto Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("WiFi conectado");

  // Configurações Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Para login anônimo (ou com email/senha)
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Enviar dados a cada 10 segundos
  if (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0) {
    sendDataPrevMillis = millis();

    float temperatura = random(200, 300) / 10.0; // Simula um sensor

    // Caminho: /estacao/temperatura
    if (Firebase.RTDB.setFloat(&fbdo, "/estacao/temperatura", temperatura)) {
      Serial.print("Temperatura enviada: ");
      Serial.println(temperatura);
    } else {
      Serial.print("Erro ao enviar: ");
      Serial.println(fbdo.errorReason());
    }
  }
}

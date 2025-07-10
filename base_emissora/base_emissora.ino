#include "ArduinoJson.h"
JsonDocument sensors;
String temp = "";

#define PIN_M0 21
#define PIN_M1 19
#define PIN_AUX 15


// SENSORS
// String direcao 1 ou 2 dígitos
// float velocidade_do_vento_kmh
// float porcentagem_umidade_do_solo
// float temperatura_ar
// float umidade_ar
// float temperatura_solo
// float chuva_mm

#include <HardwareSerial.h>
HardwareSerial LoRaSerial(2);  // UART2: RX=16, TX=17

String texto = "";

void setLoRaMode(int m0, int m1) {
  digitalWrite(PIN_M0, m0);
  digitalWrite(PIN_M1, m1);
  delay(100);
  while (digitalRead(PIN_AUX) == LOW);
  delay(2);
}

void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_AUX, INPUT);

  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Iniciando TRANSMISSOR LoRa...");
  setLoRaMode(LOW, LOW);  // Modo 0 - Transparente
  Serial.println("Modo Transparente - Pronto para transmitir.");
  
}

unsigned long lastSend = 0;
int count = 0;

void loop() {
  texto = Serial.readStringUntil('\n');  // Lê até nova linha
  if (texto != ""){
    //String message = "Mensagem #" + texto;
    String message = temp;
    LoRaSerial.println(message);
    Serial.println("Enviado: " + message);
    texto = "";
  }
  sensors["sensor"] = "gps";
  sensors["time"] = 1351824120;
  sensors["data"][0] = 78.756080;
  sensors["data"][1] = 2.302038;

  serializeJson(sensors, temp);
  Serial.println(temp);

}
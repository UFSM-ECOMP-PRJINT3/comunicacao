#define PIN_M0 21
#define PIN_M1 19
#define PIN_AUX 15

#include <HardwareSerial.h>
HardwareSerial LoRaSerial(2);  // UART2: RX=16, TX=17

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
  if (millis() - lastSend > 3000) {
    String message = "Mensagem #" + String(++count) + " fala vini!";
    LoRaSerial.println(message);
    Serial.println("Enviado: " + message);
    lastSend = millis();
  }
}
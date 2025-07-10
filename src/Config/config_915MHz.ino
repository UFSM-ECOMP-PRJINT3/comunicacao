#define PIN_M0 21
#define PIN_M1 19
#define PIN_AUX 15

#include <HardwareSerial.h>
HardwareSerial LoRaSerial(2); // UART2 (RX=16, TX=17)

void waitAUX() {
  while (digitalRead(PIN_AUX) == LOW);
  delay(2);
}

void setModeConfig() {
  digitalWrite(PIN_M0, HIGH);
  digitalWrite(PIN_M1, HIGH);
  delay(100);
  waitAUX();
}

void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_AUX, INPUT);

  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("🔧 Entrando em modo de configuração (M0=1, M1=1)...");
  setModeConfig();

  // Comando para configuração fixa: C0 00 08 ...
  byte configCommand[] = {
    0xC0,       // Comando de escrita permanente
    0x00,       // Endereço de início
    0x08,       // Tamanho dos dados
    0x00, 0x00, // ADDH, ADDL (endereço 0x0000)
    0x62,       // REG0: 9600 bps UART, 8N1, 2.4kbps air rate
    0xC0,       // REG1: sub-pacote 200 bytes, potência TX 22 dBm
    0x41,       // REG2: canal 65 (850.125 + 65 = 915.125 MHz)
    0x00,       // REG3: modo transparente, RSSI off, LBT off
    0x00, 0x00  // CHAVE (criptografia desativada)
  };

  Serial.println("🚀 Enviando configuração ideal para uso no Brasil (Canal 65, 915 MHz)...");
  LoRaSerial.write(configCommand, sizeof(configCommand));
  waitAUX();

  Serial.println("✅ Configuração aplicada com sucesso!");
  Serial.println("🔁 Agora coloque M0 = 0, M1 = 0 para operar no modo transparente.");
}

void loop() {
  // Nada necessário no loop
}

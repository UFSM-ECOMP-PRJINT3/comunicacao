#define PIN_M0 21
#define PIN_M1 19
#define PIN_AUX 15

#include <HardwareSerial.h>
HardwareSerial LoRaSerial(2); // UART2: RX=16, TX=17

void setModeConfig() {
  digitalWrite(PIN_M0, HIGH);
  digitalWrite(PIN_M1, HIGH);
  delay(100);
  while (digitalRead(PIN_AUX) == LOW);
  delay(2);
}

void waitAUX() {
  while (digitalRead(PIN_AUX) == LOW);
  delay(2);
}

void sendCommand(byte* cmd, int len) {
  LoRaSerial.write(cmd, len);
  waitAUX();

  Serial.print("Enviado: ");
  for (int i = 0; i < len; i++) Serial.print(String(cmd[i], HEX) + " ");
  Serial.println();

  delay(100);
  while (LoRaSerial.available()) {
    Serial.print("Recebido: ");
    Serial.print(LoRaSerial.read(), HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_AUX, INPUT);

  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Configurando módulo LoRa (Modo 3 - Configuração)...");
  setModeConfig();

  // Comando C0 00 08 XX XX 62 C0 00 22 (exemplo):  
  // Configura: endereço = 0x0000, serial = 9600 8N1, air rate = 2.4kbps
  // REG1 = 0xC0 => subpacote 200B, RSSI off, potência máxima (22 dBm)

  byte configCommand[] = { 0xC0, 0x00, 0x08,
                           0x00, 0x00, // ADDH, ADDL
                           0x62,       // REG0: 9600 bps, 8N1, 2.4kbps air rate
                           0xC0,       // REG1: 200B, 22 dBm
                           0x00,       // CHAN (freq = 850.125 + CH * 1MHz) => CH=0 => 850.125 MHz (ajuste se necessário)
                           0x00,       // REG3: transparent mode
                           0x00, 0x00  // encryption key
                         };

  sendCommand(configCommand, sizeof(configCommand));

  Serial.println("Configuração enviada! Volte para modo 0 (M0=0, M1=0) para usar.");
}

void loop() {
  // nada
}
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

void sendReadCommand(byte startAddress, byte length) {
  byte cmd[] = {0xC1, startAddress, length};
  LoRaSerial.write(cmd, 3);
  waitAUX();
}

void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_AUX, INPUT);

  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Entrando em modo de configuração (Modo 3)...");
  setModeConfig();

  sendReadCommand(0x00, 6); // Lê: ADDH, ADDL, REG0, REG1, REG2, REG3
}

void loop() {
  if (LoRaSerial.available() >= 9) {
    byte header = LoRaSerial.read(); // deve ser 0xC1
    byte addr = LoRaSerial.read();   // deve ser 0x00
    byte len = LoRaSerial.read();    // deve ser 0x06

    byte ADDH = LoRaSerial.read();
    byte ADDL = LoRaSerial.read();
    byte REG0 = LoRaSerial.read();
    byte REG1 = LoRaSerial.read();
    byte REG2 = LoRaSerial.read();
    byte REG3 = LoRaSerial.read();

    Serial.println("\n🛠️ Configurações do LoRa E220-900T22D:");
    Serial.printf("📟 Endereço: 0x%02X%02X\n", ADDH, ADDL);

    // UART Baudrate
    const char* uartRates[] = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
    byte uartRateBits = (REG0 >> 5) & 0b111;
    Serial.printf("🔌 UART Baudrate: %s bps\n", uartRates[uartRateBits]);

    // Paridade
    const char* parities[] = {"8N1", "8O1", "8E1", "8N1"};
    byte parityBits = (REG0 >> 3) & 0b11;
    Serial.printf("⚙️ Paridade: %s\n", parities[parityBits]);

    // Air rate
    const char* airRates[] = {"2.4kbps", "2.4kbps", "2.4kbps", "4.8kbps", "9.6kbps", "19.2kbps", "38.4kbps", "62.5kbps"};
    byte airRateBits = REG0 & 0b111;
    Serial.printf("📡 Velocidade do ar: %s\n", airRates[airRateBits]);

    // Sub-pacote
    const char* subPktSizes[] = {"200 bytes", "128 bytes", "64 bytes", "32 bytes"};
    byte subPktBits = (REG1 >> 6) & 0b11;
    Serial.printf("📦 Sub-pacote: %s\n", subPktSizes[subPktBits]);

    // Potência de transmissão
    const char* txPowers[] = {"22 dBm (máx)", "17 dBm", "13 dBm", "10 dBm"};
    byte powerBits = REG1 & 0b11;
    Serial.printf("🚀 Potência TX: %s\n", txPowers[powerBits]);

    // Canal e frequência
    byte channel = REG2;
    float freqMHz = 850.125 + channel;
    Serial.printf("📶 Canal: %d (Frequência: %.3f MHz)\n", channel, freqMHz);

    // Modo de transmissão
    const char* txModes[] = {"Transparente", "Fixado"};
    byte fixedMode = (REG3 >> 6) & 0x01;
    Serial.printf("🔁 Modo de transmissão: %s\n", txModes[fixedMode]);

    // RSSI Byte ativado?
    bool rssi = (REG3 >> 7) & 0x01;
    Serial.printf("📡 RSSI extra ativado? %s\n", rssi ? "Sim" : "Não");

    // LBT ativado?
    bool lbt = (REG3 >> 4) & 0x01;
    Serial.printf("📶 LBT ativado? %s\n", lbt ? "Sim" : "Não");

    Serial.println("\n✅ Pronto. Volte para Modo 0 (M0=0, M1=0) para operar.");
    while (true); // pausa após a leitura
  }
}

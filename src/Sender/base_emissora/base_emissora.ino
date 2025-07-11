#include "ArduinoJson.h"
#include <HardwareSerial.h>

HardwareSerial LoRaSerial(2);  // UART2: RX=16, TX=17
JsonDocument sensors;          // Declara um objeto json

String message_json = "";      // Mensagem json serializada para ser enviada
unsigned long send_time;       // Armazena o tempo em que um dado foi enviado

#define DELAY_MIN_SEND 1       // Tempo em minutos entre um envio e outro
#define SIZE_MIN 15            // Usada apenas se quiser enviar em menos de um minutos (1-60)
#define PIN_M0 21              //
#define PIN_M1 19              // Pinos do LoRa
#define PIN_AUX 15             //

float umidade_ar = 40.5;
float porcentagem_umidade_solo = 75.4;
float temperatura_ar = 31.35;
float temperatura_solo = 27.9;
float kmh = 35.32;
float chuva_mm = 120;
String direcao = "NE";
String direcoes[] = {"N", "NW", "W", "SW", "S", "SE", "E", "NE"};

// Função que seta as configurações do LoRa no setup
void setLoRaMode(int m0, int m1) {
  digitalWrite(PIN_M0, m0);
  digitalWrite(PIN_M1, m1);
  delay(100);
  while (digitalRead(PIN_AUX) == LOW);
  delay(2);
}

// Formata os dados dos sensores em um json serializado e envia pelo LoRa
void envia_dados_lora(){
  // Atribui ao objeto json os dados dos sensores
  sensors["ua"] = umidade_ar;
  sensors["us"] = porcentagem_umidade_solo;
  sensors["ta"] = temperatura_ar;
  sensors["ts"] = temperatura_solo;
  sensors["vv"] = kmh;
  sensors["cm"] = chuva_mm;
  sensors["di"] = direcao;

  // Serializa o json
  serializeJson(sensors, message_json);
  // Envia o json serializado pelo LoRa
  LoRaSerial.println(message_json);
  Serial.println("Enviado: " + message_json);
}

void setup() {

  randomSeed(analogRead(0));
  Serial.begin(115200);

  // Pinos do LoRa
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_AUX, INPUT);

  // Inicializa o LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("Iniciando TRANSMISSOR LoRa...");
  setLoRaMode(LOW, LOW);  // Modo 0 - Transparente
  Serial.println("Modo Transparente - Pronto para transmitir.");

  // Salva o tempo em que terminou o setup para começar a contagem do tempo de envio
  send_time = millis();
}

void loop() {

  // Temporizador para envio de dados
  if (millis() - send_time >= DELAY_MIN_SEND*1000*SIZE_MIN){
    // Envia os dados e reinicia o temporizador
    envia_dados_lora();
    send_time = millis();

    umidade_ar = random(200, 1000) / 10.0;              // 20.0 a 100.0 %
    porcentagem_umidade_solo = random(100, 1000) / 10.0; // 10.0 a 100.0 %
    temperatura_ar = random(150, 450) / 10.0;            // 15.0 a 45.0 °C
    temperatura_solo = random(100, 400) / 10.0;          // 10.0 a 40.0 °C
    kmh = random(0, 1000) / 10.0;                        // 0.0 a 100.0 km/h
    chuva_mm = random(0, 200);                          // 0 a 200 mm
    direcao = direcoes[random(0, 8)];
    
  }
}



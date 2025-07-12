# 🌦️ CloudIA – Comunicação para Estação Meteorológica

Este projeto tem como objetivo desenvolver e implementar um sistema robusto de comunicação para uma estação meteorológica, utilizando microcontroladores ESP32 e módulos LoRa E220, com envio dos dados coletados para um banco de dados remoto InfluxDB.

## 📦 Estrutura do Projeto

- `base_emissora.ino` – Código da base que emite os dados via LoRa coletados pelos sensores na estação
- `base_receptora.ino` – Código da base que recebe os dados via LoRa e envia ao Firebase  
- `arduino_sender_msg.ino` – Código de teste de envio de mensagem via LoRa  
- `arduino_recieve_msg.ino` – Código de teste de recepção de mensagem via LoRa 
- `config_22db.ino` – Configuração dos módulos LoRa para potência de 22 dB  
- `config_915MHz.ino` – Configuração da frequência dos módulos LoRa para 915 MHz  
- `get_config.ino` – Código de leitura dos parâmetros dos módulos LoRa  
- `CloudIA.stl` – Modelo 3D da caixa protetora personalizada para ESP32 + LoRa  
- `README.md` – Documentação geral do projeto  

## 🛠️ Tecnologias Utilizadas

- **Microcontroladores:** ESP32 
- **Comunicação de longa distância:** LoRa E220 915MHz  
- **Banco de dados:** Firebase
- **Protocolos:** HTTP / LoRa  
- **Formato de dados:** JSON via biblioteca [ArduinoJson](https://arduinojson.org/)  

## 🧠 Funcionamento Geral

1. **Coleta de Dados**  
   O ESP32 conectado a sensores meteorológicos coleta dados como temperatura, umidade, pressão, etc.

2. **Transmissão**  
   - Os dados coletados são enviados da estação para uma base por **LoRa**;

3. **Receptor**  
   O segundo módulo ESP32 + LoRa, conectado à internet via Wi-Fi, recebe os dados e os envia para o banco de dados **Firebase**.

## 📡 Arquitetura do Sistema

```
[Sensores + ESP32 + LoRa] --- (até 10 km) ---> [ESP32 + LoRa + Wi-Fi] ---> [Servidor Firebase]
```

## 🧱 Caixa Impressa em 3D

Foi projetada e impressa uma **caixa protetora personalizada em 3D** para acomodar o conjunto ESP32 + LoRa, garantindo robustez e proteção contra intempéries.  
O modelo STL está disponível neste repositório: [`CloudIA.stl`](CloudIA.stl)

## 📊 Envio de Dados

- Os dados são encapsulados em formato JSON;  
- Cada pacote inclui carimbo de tempo;  
- Envio via API Firebase ESP32 Client para o banco de dados Firebase.  

## 🧪 Testes Realizados

- Estabilidade da conexão Wi-Fi e reconexão automática  
- Comunicação via LoRa a diversas distâncias  
- Consumo de energia com uso de modo sleep  
- Testes de integridade dos pacotes transmitidos  
- Validação da estrutura do banco de dados e dashboards  

## 🧠 Desafios Enfrentados e Soluções

| Desafio                        | Solução Adotada / Sugestão                                        |
|-------------------------------|--------------------------------------------------------------------|
| Falta de Wi-Fi no local       | ✅ Uso de módulos LoRa E220                                       |
| Alto consumo de energia       | 🔧 Sugestão futura: Implementação de `deep sleep` no ESP32        |
| Perda de pacotes              | 🔧 Sugestão futura: Reenvio automático e verificação CRC          |
| Segurança na transmissão      | 🔧 Sugestão futura: Implementação de criptografia AES              |
| Integração com banco de dados | ✅ Firebase usado com autenticação por token                       |
| Integração com InfluxDB       | ❌ Não implementado (foi priorizado o Firebase)                    |

## 👨‍💻 Equipe

- Bruno Henrique Spies  
- Mathias Cirolini Michelotti  
- Vinícius Gabriel Schultz  
- Pedro Augusto Pötter  
- **Orientador:** Prof. Carlos Henrique Barriquelo

## 📚 Recursos Úteis

- Firebase ESP32 Client:
  https://github.com/mobizt/Firebase-ESP-Client/
- Biblioteca ArduinoJson:
  https://arduinojson.org/

## 🎥 Demonstração em Vídeo

Link para vídeo de apresentação/testes do projeto:  
[🔗 Vídeo no Google Drive](https://drive.google.com/file/d/1NMRpJQ5XkV_NKbAByBrYIIFYPvqY6l8L/view?usp=sharing)

## 🧪 Licença

Este projeto é de uso acadêmico e experimental no contexto da disciplina do curso de Engenharia da Computação - UFSM.

# 🌦️ CloudIA – Comunicação para Estação Meteorológica

Este projeto tem como objetivo desenvolver e implementar um sistema robusto de comunicação para uma estação meteorológica, utilizando microcontroladores ESP32 e módulos LoRa E220, com envio dos dados coletados para um banco de dados remoto InfluxDB.

## 📦 Estrutura do Projeto

- `base_emissora.ino` – Código da base que recebe os dados via LoRa  
- `arduino_sender_msg.ino` – Código do nó sensor que envia mensagens  
- `arduino_recieve_msg.ino` – Código para testes de recepção LoRa  
- `config_22db.ino` e `config_915MHz.ino` – Configuração dos módulos LoRa  
- `get_config.ino` – Leitura de parâmetros dos módulos LoRa  
- `CloudIA.stl` – Arquivo STL do modelo 3D da caixa protetora impressa para o conjunto ESP32 + LoRa  

## 🛠️ Tecnologias Utilizadas

- **Microcontroladores:** ESP32 / ESP8266  
- **Comunicação de longa distância:** LoRa E220 915MHz  
- **Banco de dados:** InfluxDB  
- **Protocolos:** HTTP / MQTT / LoRa  
- **Formato de dados:** JSON via biblioteca [ArduinoJson](https://arduinojson.org/)  

## 🧠 Funcionamento Geral

1. **Coleta de Dados**  
   O ESP32 conectado a sensores meteorológicos coleta dados como temperatura, umidade, pressão, etc.

2. **Transmissão**  
   - Se houver Wi-Fi disponível, os dados são enviados diretamente via HTTP/MQTT.  
   - Caso contrário, os dados são transmitidos via **LoRa** para um gateway remoto (até 10 km de distância).

3. **Receptor**  
   O segundo módulo ESP32 + LoRa, conectado à internet via Wi-Fi, recebe os dados e os envia para o banco de dados **InfluxDB**.

4. **Visualização**  
   Os dados são processados e exibidos em dashboards interativos para análise.

## 📡 Arquitetura do Sistema

```
[Sensores + ESP32 + LoRa] --- (até 10 km) ---> [ESP32 + LoRa + Wi-Fi] ---> [Servidor InfluxDB]
```

## 🧱 Caixa Impressa em 3D

Foi projetada e impressa uma **caixa protetora personalizada em 3D** para acomodar o conjunto ESP32 + LoRa, garantindo robustez e proteção contra intempéries.  
O modelo STL está disponível neste repositório: [`CloudIA.stl`](CloudIA.stl)

## 📊 Envio de Dados

- Os dados são encapsulados em formato JSON  
- Cada pacote inclui carimbo de tempo  
- Envio via API REST para o banco InfluxDB  

## 🧪 Testes Realizados

- Estabilidade da conexão Wi-Fi e reconexão automática  
- Comunicação via LoRa a diversas distâncias  
- Consumo de energia com uso de modo sleep  
- Testes de integridade dos pacotes transmitidos  
- Validação da estrutura do banco de dados e dashboards  

## 🧠 Desafios Enfrentados e Soluções

| Desafio                        | Solução Adotada                      |
|-------------------------------|--------------------------------------|
| Falta de Wi-Fi no local       | Uso de módulos LoRa E220             |
| Alto consumo de energia       | Uso de `deep sleep` no ESP32         |
| Perda de pacotes              | Reenvio automático de pacotes        |
| Segurança na transmissão      | Implementação de criptografia        |
| Integração com servidor       | Testes incrementais com InfluxDB     |

## 👨‍💻 Equipe

- Bruno Henrique Spies  
- Mathias Cirolini Michelotti  
- Vinícius Gabriel Schultz  
- Pedro Augusto Pötter  
- **Orientador:** Prof. Carlos Henrique Barriquelo

## 📚 Recursos Úteis

- Biblioteca LoRa:  
  https://github.com/xreef/EByte_LoRa_E220_Series_Library  
- Biblioteca ArduinoJson:  
  https://arduinojson.org/

## 🎥 Demonstração em Vídeo

Link para vídeo de apresentação/testes do projeto:  
[🔗 Vídeo no Google Drive](https://drive.google.com/file/d/1NMRpJQ5XkV_NKbAByBrYIIFYPvqY6l8L/view?usp=sharing)

## 🧪 Licença

Este projeto é de uso acadêmico e experimental no contexto da disciplina do curso de Engenharia da Computação - UFSM.

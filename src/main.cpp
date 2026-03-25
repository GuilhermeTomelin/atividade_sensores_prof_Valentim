#include <Arduino.h>

// 1. Incluindo os arquivos que você já possui na pasta 'src'
#include "PIR.h"
#include "sensor_agua.h"
#include "sensor_gas.h"

// 2. Adicionando Temperatura e Umidade (Exemplo usando a biblioteca DHT)
// Se não tiver a biblioteca instalada, adicione "adafruit/DHT sensor library" no platformio.ini
#include "DHT.h"

#define DHTPIN 4          // Pino digital onde o sensor DHT está conectado
#define DHTTYPE DHT11     // Mude para DHT22 se for o seu caso
DHT dht(DHTPIN, DHTTYPE);

// Configurações de Alerta
const int PIN_ALERTA = 2; // Pino do LED ou Buzzer de alerta
const float LIMITE_TEMP = 35.0; // Exemplo de temperatura instável

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(PIN_ALERTA, OUTPUT);

    // Inicialize aqui as funções de setup dos seus outros arquivos, se houver:
    // setupPIR(); 
    // setupAgua();
    
    Serial.println("Sistema de Monitoramento Iniciado...");
}

void loop() {
    // Leituras de Temperatura e Umidade
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Verificação de erro na leitura do DHT
    if (isnan(h) || isnan(t)) {
        Serial.println("Erro ao ler o sensor de Temperatura/Umidade!");
    }

    // Variável para verificar se algum sensor disparou alerta
    bool sistemaEmAlerta = false;

    // Lógica de Verificação (Chamando funções que devem estar nos seus .h)
    // Ajuste os nomes das funções conforme você as escreveu nos arquivos .h
    if (verificarMovimento() || verificarVazamentoAgua() || verificarGas() || t > LIMITE_TEMP) {
        sistemaEmAlerta = true;
    }

    // Ação de Alerta
    if (sistemaEmAlerta) {
        digitalWrite(PIN_ALERTA, HIGH);
        Serial.print("--- ALERTA ATIVADO ---");
        Serial.print(" Temp: "); Serial.print(t);
        Serial.print(" Hum: "); Serial.println(h);
        // TODO: Enviar notificação para App/PC aqui
    } else {
        digitalWrite(PIN_ALERTA, LOW);
    }

    delay(2000); // Aguarda 2 segundos para a próxima leitura
}
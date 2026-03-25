#include<Arduino.h>

const int pirPin = 7;
const int ledPin = 8;

unsigned long motionStartTime = 0;  // quando o movimento começou
const unsigned long triggerTime = 2000; // 5 segundos em milissegundos

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int pirState = digitalRead(pirPin);

  if (pirState == HIGH) {
    // Movimento detectado
    if (motionStartTime == 0) {
      // Começa a contar o tempo do movimento
      motionStartTime = millis();
    } else {
      // Já estamos contando, verifica se atingiu 5 segundos
      if (millis() - motionStartTime >= triggerTime) {
        digitalWrite(ledPin, HIGH); // Aciona o LED
      }
    }
  } else {
    // Sem movimento, reinicia a contagem
    motionStartTime = 0;
    digitalWrite(ledPin, LOW); // Desliga o LED
  }

  delay(10); // Pequeno atraso para estabilidade
}
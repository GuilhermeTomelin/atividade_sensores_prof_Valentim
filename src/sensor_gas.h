#include <arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuração dos Pinos
const int pinoGas = A5;
const int ledVerde = 2;    // Agora no pino 2
const int ledVermelho = 3; // Agora no pino 3

void setup() {
  lcd.init();
  lcd.backlight();
 
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Monitor de Gas");
  lcd.setCursor(0, 1);
  lcd.print("Lado 2 e 3");
  delay(2000);
  lcd.clear();
}

void loop() {
  int nivelGas = analogRead(pinoGas);

  // Exibição no LCD
  lcd.setCursor(0, 0);
  lcd.print("Nivel: ");
  lcd.print(nivelGas);
  lcd.print(" PPM    ");

  // --- Lógica: BOM se for menos de 1000 ---
 
  if (nivelGas < 1000) {
    // STATUS: BOM
    lcd.setCursor(0, 1);
    lcd.print("STATUS: BOM     ");
   
    digitalWrite(ledVerde, HIGH);   // Liga Verde
    digitalWrite(ledVermelho, LOW); // Desliga Vermelho
  }
  else {
    // STATUS: PERIGO (Acima de 1000)
    lcd.setCursor(0, 1);
    lcd.print("STATUS: PERIGO! ");
   
    digitalWrite(ledVerde, LOW);    // Desliga Verde
   
    // Pisca o Vermelho para chamar atenção
    digitalWrite(ledVermelho, HIGH);
    delay(200);
    digitalWrite(ledVermelho, LOW);
    delay(200);
  }

  delay(200); // Leitura rápida para resposta imediata
}
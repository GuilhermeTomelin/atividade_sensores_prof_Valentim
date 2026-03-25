#include <Wire.h>                      // Biblioteca para comunicação I2C
#include <LiquidCrystal_I2C.h>         // Biblioteca do LCD I2C

// Inicializa o display no endereço 0x27 (ou 0x3F), com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();                          // Inicializa o LCD
  lcd.backlight();                     // Liga a luz de fundo
  lcd.setCursor(0, 0);                 // Define cursor para coluna 0, linha 0g
  lcd.print("Ola, Mundo!");            // Escreve no display
}


void loop() {
  // Nada aqui
}

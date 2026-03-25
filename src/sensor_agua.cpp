#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo bombaServo;

const int pinServo = 9;       
const int pinReleLed = 12;    
const int pinPotVazao = A0;   

float volumeAtual = 0.0;      
bool estadoBomba = false;     
bool sensorBloqueou = false; 
float totalAcumulado = 0.0;   
unsigned long ultimoTempoSave = 0;
int porcentagemAnterior = -1; 

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(pinReleLed, OUTPUT);    
  bombaServo.attach(pinServo); 
  
  EEPROM.get(0, totalAcumulado);
  EEPROM.get(10, volumeAtual);
  
  if (isnan(totalAcumulado) || totalAcumulado < 0) totalAcumulado = 0.0;
  if (isnan(volumeAtual) || volumeAtual < 0) volumeAtual = 0.0;

  bombaServo.write(0);         
}

void loop() {
  long somaPot = 0;
  for(int i=0; i<10; i++) somaPot += analogRead(pinPotVazao);
  int leituraPot = somaPot / 10;

  float volumeAlvo = (leituraPot / 1023.0) * 9000.0; 
  int anguloServo = map(leituraPot, 0, 1023, 0, 180);

  if (volumeAtual >= 9000) sensorBloqueou = true;
  else sensorBloqueou = false;

  if (!sensorBloqueou && volumeAtual < (volumeAlvo - 50)) {
    estadoBomba = true;
    volumeAtual += 100.0; 
    totalAcumulado += 100.0;
    if (volumeAtual > 9000) volumeAtual = 9000; 
  } 
  else if (volumeAtual > (volumeAlvo + 50)) {
    estadoBomba = false;
    volumeAtual -= 100.0;
    if (volumeAtual < 0) volumeAtual = 0; 
  }
  else {
    estadoBomba = false;
  }

  if (estadoBomba) {
    digitalWrite(pinReleLed, HIGH); 
    bombaServo.write(anguloServo);  
  } else {
    digitalWrite(pinReleLed, LOW);  
    bombaServo.write(0);            
  }

  int nivelLCD = (volumeAtual / 1800.0); 
  if (nivelLCD > 5) nivelLCD = 5;
  if (volumeAtual >= 8950) nivelLCD = 5; 

  int porcentagem = (volumeAtual / 10000.0) * 100;
  
  if (porcentagem != porcentagemAnterior) {
    lcd.setCursor(0, 0);
    lcd.print("N:"); lcd.print(nivelLCD);
    lcd.print(" V:"); lcd.print((long)volumeAtual);
    lcd.print("    "); 

    lcd.setCursor(11, 0); 
    lcd.print("    ");    
    lcd.setCursor(11, 0);
    lcd.print(porcentagem); lcd.print("%");
    
    porcentagemAnterior = porcentagem; 
  }

  lcd.setCursor(0, 1);
  lcd.print("A:"); lcd.print((long)totalAcumulado);
  lcd.print(" Bmb:");
  if (volumeAtual >= 9000) lcd.print("BLQ");
  else lcd.print(estadoBomba ? "ON " : "OFF");

  // Salvamento EEPROM
  if(millis() - ultimoTempoSave > 3000) {
    EEPROM.put(0, totalAcumulado);
    EEPROM.put(10, volumeAtual);
    ultimoTempoSave = millis();
  }
  
  delay(100); 
}

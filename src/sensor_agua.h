#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Servo.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo bombaServo;                  


const int pinServo = 9;      
const int pinReleLed = 12;    
const int pinPotVazao = A0;  


const int pinChave1 = 2;
const int pinChave2 = 3;
const int pinChave3 = 4;
const int pinChave4 = 5;
const int pinChave5 = 6;


float volumeAtual = 0.0;      
bool estadoBomba = false;    
bool sensorBloqueou = false;
float totalAcumulado = 0.0;  
unsigned long ultimoTempoSave = 0;
unsigned long ultimoTempoLCD = 0;
bool estavaEmErro = false;        


void setup() {
  lcd.init();
  lcd.backlight();


  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(5000);
  lcd.clear();


  pinMode(pinReleLed, OUTPUT);    
  bombaServo.attach(pinServo);
 
  pinMode(pinChave1, INPUT);
  pinMode(pinChave2, INPUT);
  pinMode(pinChave3, INPUT);
  pinMode(pinChave4, INPUT);
  pinMode(pinChave5, INPUT);
 
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


  float limiteVolume = 0.0;
 
  if (digitalRead(pinChave5))      limiteVolume = 9000.0;
  else if (digitalRead(pinChave4)) limiteVolume = 8000.0;
  else if (digitalRead(pinChave3)) limiteVolume = 6000.0;
  else if (digitalRead(pinChave2)) limiteVolume = 4000.0;
  else if (digitalRead(pinChave1)) limiteVolume = 2000.0;
  else                             limiteVolume = 0.0;    


  if (volumeAlvo > limiteVolume) {
    volumeAlvo = limiteVolume;
  }


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


  bool c1 = digitalRead(pinChave1);
  bool c2 = digitalRead(pinChave2);
  bool c3 = digitalRead(pinChave3);
  bool c4 = digitalRead(pinChave4);
  bool c5 = digitalRead(pinChave5);


  bool erroChaves = false;
  if (c5 && (!c4 || !c3 || !c2 || !c1)) erroChaves = true;
  else if (c4 && (!c3 || !c2 || !c1)) erroChaves = true;
  else if (c3 && (!c2 || !c1)) erroChaves = true;
  else if (c2 && !c1) erroChaves = true;


  int nivelLCD = 0;
  int porcentagem = 0;
  if (!erroChaves) {
    if (c5)      { nivelLCD = 5; porcentagem = 90; }
    else if (c4) { nivelLCD = 4; porcentagem = 80; }
    else if (c3) { nivelLCD = 3; porcentagem = 50; }
    else if (c2) { nivelLCD = 2; porcentagem = 30; }
    else if (c1) { nivelLCD = 1; porcentagem = 10; }
    else         { nivelLCD = 0; porcentagem = 0; }
  }


  if (millis() - ultimoTempoLCD > 250) {
    ultimoTempoLCD = millis();


    if (erroChaves) {
      lcd.setCursor(0, 0);
      lcd.print("ERRO NOS NIVEIS!");
      lcd.setCursor(0, 1);
      lcd.print("VERIFIQUE CHAVES");
      estavaEmErro = true;
    }
    else {
      if (estavaEmErro) {
        lcd.clear();
        estavaEmErro = false;
      }


      lcd.setCursor(0, 0);
      lcd.print("N:"); lcd.print(nivelLCD);
      lcd.print(" V:"); lcd.print((long)volumeAtual);
      lcd.print("   ");
     
      lcd.setCursor(12, 0);            
      if (porcentagem < 100) lcd.print(" ");
      if (porcentagem < 10)  lcd.print(" ");
      lcd.print(porcentagem); lcd.print("%");


      lcd.setCursor(0, 1);
      lcd.print("A:"); lcd.print((long)totalAcumulado);
      
      lcd.print(" Bmb:");
      if (volumeAtual >= 9000) lcd.print("BLQ");
      else lcd.print(estadoBomba ? "ON " : "OFF");
    }
  }


  if(millis() - ultimoTempoSave > 3000) {
    EEPROM.put(0, totalAcumulado);
    EEPROM.put(10, volumeAtual);
    ultimoTempoSave = millis();
  }
 
  delay(30);
}

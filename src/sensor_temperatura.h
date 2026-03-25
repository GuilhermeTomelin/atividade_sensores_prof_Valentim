// C++ code
// Projeto Sensor LM35 

const int LM35 = A1; // Define q o sensor está no pino analog A1
float temperatura; // Variável que armazenará a tempeatura medida

void setup(){
Serial.begin (9600); //Inicia a comunic SERIAL
}

void loop(){
//Fórm convers. para Celsius
temperatura =(float(analogRead(LM35))*5/(1023))/0.01;
Serial.print("Temperatura: ");
Serial.println(temperatura); //Imprime no monitor a temperatura lida
delay (3000);
}

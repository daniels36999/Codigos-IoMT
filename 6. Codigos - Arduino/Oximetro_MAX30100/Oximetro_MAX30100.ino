#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define TiempoMuestreo     1000

#define Control 3
#define Led 2

int Resultado=0;
int Lectura=0;

PulseOximeter pox;
uint32_t TiempoAnterior = 0;
void onBeatDetected()
{
   Serial.println("Inicio");
}
 
void setup(){
    Serial.begin(9600);
    Wire.begin();
    pinMode(Control,INPUT);
    pinMode(Led,OUTPUT);
    digitalWrite(Led,HIGH);
    if (!pox.begin()) {
        Serial.println("Falla en el Oximetro ");
        for(;;);} 
    else {
        Serial.println("Oximetro Detectado.");}
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
     pox.setOnBeatDetectedCallback(onBeatDetected);
     digitalWrite(Led,LOW);
}
 
void loop(){
      pox.update();
  if (digitalRead(Control)==HIGH){
      digitalWrite(Led,HIGH);
      if (millis() - TiempoAnterior > TiempoMuestreo) {
          //Serial.println(pox.getSpO2());
          Resultado=pox.getSpO2();
          Wire.beginTransmission(1);  // Transmite al Esclavo 1
          Wire.write(Resultado);
          Wire.endTransmission();
          TiempoAnterior = millis();}}
          
  else if (digitalRead(Control)==LOW){
    digitalWrite(Led,LOW);}
}

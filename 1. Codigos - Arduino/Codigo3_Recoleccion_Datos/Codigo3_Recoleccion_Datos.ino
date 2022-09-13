//DECLARACION DE LIBRERIAS
#include <Wire.h>
#include "HX711.h"
#include "TM1637.h" 
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <TM1637Display.h>
#include <Adafruit_MLX90614.h>
#include <Ticker.h>

//DECLARACION DE PINES - TEMPERATURA
#define DispTemClk A0  //PIN DISPLAY1 - CLK
#define DispTemDio A1  //PIN DISPLAY1 - DIO
#define UltTempTrig 18 //PIN ULTRASONICO1 - TRIG
#define UltTempEcho 19 //PIN ULTRASONICO1 - ECHO
#define LDVeTemp 53    //PIN LED VERDE
#define LDAzTemp 22    //PIN LED AZUL
#define ReleRGBTemV 23 //PIN TIRA LED VERDE
#define ReleRGBTemR 25 //PIN TIRA LED ROJO
#define BuzzTemp 2     //PIN BUZZER

//DECLARACION DE PINES -  BASCULA
#define DispBasClk A2 //PIN DISPLAY2 - CLK
#define DispBasDio A3 //PIN DISPLAY2 - DIO
#define UltBasTrig 15 //PIN ULTRASONICO2 - TRIG
#define UltBasEcho 14 //PIN ULTRASONICO2 - ECHO
#define ModBasDT A11  //PIN BASCULA - DATA
#define ModBasCSK A10 //PIN BASCULA - CSK
#define LDVeBas 51    //LED VERDE
#define BuzzBas 24    //BUZZER BACULA

//DECLARACION DE PINES -  ALTURA
#define DispAltClk A4  //PIN DISPLAY3 - CLK
#define DispAltDio A5  //PIN DISPLAY3 - DIO
#define UltAltTrig 17  //PIN ULTRASONICO3 - TRIG
#define UltAltEcho 16  //PIN ULTRASONICO3 - ECHO
#define LDAzAlt 26     //PIN LED AZUL
#define LDVeAlt 49     //LED VERDE

//DECLARACION DE PARAMETROS OXIMETRO
#define DispOxiClk A8 //PIN DISPLAY4 - CLK
#define DispOxiDio A9 //PIN DISPLAY4 - DIO
#define UltOxiTrig 8 //PIN ULTRASONICO4 - TRIG
#define UltOxiEcho 9 //PIN ULTRASONICO4 - ECHO
#define LDVeOxi 45  //LED VERDE 
#define LDAzOxi 10  //LED AZUL
#define BuzzOxi 11  //BUZZER OXIMETRO
#define ActivaOxi 12

//DECLARACION DE PARAMETROS IMC
#define IMCClk A6  //PIN DISPLAY5 - CLK
#define IMCDio A7  //PIN DISPLAY5 - DIO
#define LDVeIMC 47 //LED VERDE 

//VARIABLES RESULTANTES DE EVALUACION FISICA
float ResTemp, ResTemperatura=0;
float ResPes, ResPeso=0;
float DistAlt=0, ResAltura=0, ResEst, ResEstatura=0;
float ResOxi, ResOximetro=0;
float ResIMC;
char c='F';

//CONDICIONES DE LOS SENSORES
int DistTemp=0,     DistBas=0,      DistAlt2=0,    DistOxi=0;
int DistMaxTemp=5, DistMaxBas=160, DistMaxAlt=160, DistMaxOxi=5;
int TempMax=37.2, TempMin=30;

//PARAMETROS DEL TIEMPO PARA EL PROCESO
int Tiempo=0,  TDisp1=10000,    TEnv=200;
int Estado1=0, UltimoCambio1=0, TiempoCambio1=0;  
int Estado2=0, UltimoCambio2=0, TiempoCambio2=0; 
int Estado3=0, UltimoCambio3=0, TiempoCambio3=0; 
int Estado4=0, UltimoCambio4=0, TiempoCambio4=0; 
int Estado5=0, UltimoCambio5=0, TiempoCambio5=0; 
int Estado6=0, UltimoCambio6=0, TiempoCambio6=0;
int EstTemperatura =0, UlTCambTemp=0,   TCambTemp=0; 

//DECLARACION DE INICIALES - MODULOS
HX711 Bascula;
TM1637Display DispTemperatura(DispTemClk, DispTemDio);
TM1637Display DispAltura(DispAltClk, DispAltDio);
TM1637Display DispBascula(DispBasClk, DispBasDio);
TM1637Display DispOximetro(DispOxiClk, DispOxiDio);
TM1637Display DispIMC(IMCClk, IMCDio);
Adafruit_MLX90614 Termometro = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Condicion_Pines();
  Wire.begin(1);//OXIMETRO
  Termometro.begin(0x5A);
  Bascula.begin(ModBasDT, ModBasCSK);
  Wire.onReceive(Oximetria);

  DispTemperatura.setBrightness(6);
  DispAltura.setBrightness(6);
  DispBascula.setBrightness(6);
  DispOximetro.setBrightness(6);
  DispIMC.setBrightness(6);

  Inicio_Parametros();
  Inicio_Bascula();
}

void loop() {

  TiempoSistema();
  Ultrasonico_Temperatura();
  Ultrasonico_Bascula();
  Ultrasonico_Altura();
  Ultrasonico_Oximetro();
  Leer();
  
  //PROECESO - TEMPERATURA
  //-----------------------------------------
  if(DistTemp<=DistMaxTemp){
    Temperatura_Corporal();
    Vizualizar_Temperatura(ResTemperatura);
    Condicion_Temperatura();
    digitalWrite(LDVeTemp,HIGH);
    Estado1=0;}
  else{
    digitalWrite(BuzzTemp, LOW);
    switch (Estado1){
    case 0:
      if(TiempoCambio1>=10000){
        UltimoCambio1=Tiempo;
        Vizualizar_Temperatura(ResTemperatura);
        Condicion_Temperatura();
        digitalWrite(LDVeTemp,HIGH);
        Estado1=1;}
    break;  
    case 1:
      if(TiempoCambio1>=1000){
        UltimoCambio1=Tiempo;  
        DispTemperatura.showNumberDec(0,false);
        digitalWrite(ReleRGBTemV,HIGH);
        digitalWrite(ReleRGBTemR,HIGH);
        digitalWrite(LDVeTemp,LOW);
        ResTemperatura=0;
        Estado1=1;}
    break;}}
  
  //PROECESO - BASCULA E IMC
  //-----------------------------------------
  if(DistBas<=DistMaxBas||DistAlt<=DistMaxBas){
    Balanza();
    IMC();
    Vizualizar_Bascula(ResPeso);
    Vizualizar_IMC(ResIMC);
    digitalWrite(LDVeBas,HIGH);
    digitalWrite(LDVeIMC,HIGH);
    Estado2=0;}
  else{
  switch (Estado2){
    case 0:
      if(TiempoCambio2>=5000){
        UltimoCambio2=Tiempo;
        Vizualizar_IMC(ResIMC);
        Vizualizar_Bascula(ResPeso);
        digitalWrite(LDVeBas,HIGH);
        digitalWrite(LDVeIMC,HIGH);
        Estado2=1;}
    break;  
    case 1:
      if(TiempoCambio2>=1000){
        UltimoCambio2=Tiempo;
        DispBascula.showNumberDec(0,false);
        DispIMC.showNumberDec(0,false);
        digitalWrite(LDVeBas,LOW);
        digitalWrite(LDVeIMC,LOW);
        ResPeso=0;
        Estado2=1;}
    break; } }

  //PROECESO - ALTURA
  //----------------------------------------- 
  if(DistAlt<=DistMaxAlt){
    Estatura();
    Vizualizar_Altura(ResEstatura);
    digitalWrite(LDVeAlt,HIGH);
    Estado3=0; }
  else{
      switch (Estado3){
    case 0:
      if(TiempoCambio3>=8000){
        UltimoCambio3=Tiempo;
        Vizualizar_Altura(ResEstatura);
        digitalWrite(LDVeAlt,HIGH);
        Estado3=1;}
    break;  
    case 1:
      if(TiempoCambio3>=1000){
        UltimoCambio3=Tiempo;
        DispAltura.showNumberDec(0,false);
        digitalWrite(LDVeAlt,LOW);
        ResEstatura=0;
        Estado3=1;}
    break; } }
    
  //PROECESO - OXIMETRO
  //-----------------------------------------
  if(DistOxi<=DistMaxOxi){
    Oximetria();
    digitalWrite(ActivaOxi , HIGH);
    Vizualizar_Oximetro(ResOximetro);
    digitalWrite(LDVeOxi,HIGH);
    Estado4=0;  } 
  else{
    digitalWrite(ActivaOxi , LOW);
      switch (Estado4){
    case 0:
      if(TiempoCambio4>=10000){
        UltimoCambio4=Tiempo;
        Vizualizar_Oximetro(ResOximetro);
        digitalWrite(LDVeOxi,HIGH);
        Estado4=1;}
    break;  
    case 1:
      if(TiempoCambio4>=1000){
        UltimoCambio4=Tiempo;
        DispOximetro.showNumberDec(0,false);
        digitalWrite(LDVeOxi,LOW);
        ResOximetro=0;
        Estado4=1;}
    break; } }

//ENVIAR DATOS A LA TARJETA RASPBERRY PI 4
switch (Estado5){
    case 0:
      if(TiempoCambio5>=TEnv){
        UltimoCambio5=Tiempo;
        Serial.println(String(ResTemperatura)+','+String(ResPeso)+','+String(ResEstatura)+','+String(1) +','+String(ResOximetro));
        Estado5=1;}
    break;  
    case 1:
      if(TiempoCambio5>=TEnv){
        UltimoCambio5=Tiempo;
        Estado5=0;}
    break; }
}

//TIEMPO DEL SISTEMA
void TiempoSistema(){
  Tiempo=millis();
  TiempoCambio1=Tiempo-UltimoCambio1;
  TiempoCambio2=Tiempo-UltimoCambio2;
  TiempoCambio3=Tiempo-UltimoCambio3;
  TiempoCambio4=Tiempo-UltimoCambio4;
  TiempoCambio5=Tiempo-UltimoCambio5;
  TiempoCambio6=Tiempo-UltimoCambio6;
  TCambTemp=Tiempo-UlTCambTemp; 
  }
  
//FUNCION RECOLECTAR - TEMPERATURA
void Temperatura_Corporal(){
  ResTemp = Termometro.readObjectTempC();//RESULTADO TEMPERATURA CORPORA
    if(ResTemperatura<ResTemp){
        ResTemperatura=ResTemp; }
   }

void Condicion_Temperatura(){
  if(ResTemperatura  <= TempMin){
    digitalWrite(ReleRGBTemV,HIGH);
    digitalWrite(ReleRGBTemR,HIGH);}
  else if (ResTemperatura  > TempMin&&ResTemperatura  < TempMax) {
    digitalWrite(ReleRGBTemV,LOW);
    digitalWrite(ReleRGBTemR,HIGH);}
  else if (ResTemperatura  >= TempMax) {
    Buzzer_Temperatura();
    digitalWrite(ReleRGBTemV,HIGH);
    digitalWrite(ReleRGBTemR,LOW);}
  }

//FUNCION RECOLECTAR - ESTATURA
void Estatura(){
  ResAltura=1.90-(DistAlt/100);
  if(ResAltura<=0){
   ResEstatura=0;}
  else{
    if(ResEstatura<ResAltura){
        ResEstatura=ResAltura;} } 
   }
        
//FUNCION RECOLECTAR - BASCULA
void Balanza(){
  ResPes=abs(Bascula.get_units(2));
  if(ResPes<0){
    ResPes=(ResPes)*(-1);}
  else{
    ResPeso=ResPes; }  
  }

//FUNCION RECOLECTAR - OXIMETRO
void Oximetria(){
  if (Wire.available() == 1) {
      ResOxi= Wire.read();  }
      if(ResOximetro<ResOxi){
          ResOximetro=ResOxi; } 
   }

//FUNCION RECOLECTAR - IMC
void IMC(){
  ResIMC=ResPeso/(ResEstatura*ResEstatura); 
  }
  
//ULTRASONICO TEMPERATURA
void Ultrasonico_Temperatura(){         
  long Duracion, Distancia; 
  digitalWrite(UltTempTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(UltTempTrig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(UltTempTrig, LOW);
  Duracion = pulseIn(UltTempEcho, HIGH);
  Distancia = (Duracion/2) / 29;
  DistTemp=Distancia; 
  if(Distancia <=DistMaxTemp){
    digitalWrite(LDAzTemp,HIGH);}
  else{
    digitalWrite(LDAzTemp,LOW);}
  }

void Ultrasonico_Bascula(){                   
  long Duracion, Distancia; 
  digitalWrite(UltBasTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(UltBasTrig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(UltBasTrig, LOW);
  Duracion = pulseIn(UltBasEcho, HIGH);
  Distancia = (Duracion/2) / 29; 
  DistBas=Distancia;
  }
  
//ULTRASONICO ALTURA
void Ultrasonico_Altura(){                   
  long Duracion, Distancia; 
  digitalWrite(UltAltTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(UltAltTrig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(UltAltTrig, LOW);
  Duracion = pulseIn(UltAltEcho, HIGH);
  Distancia = (Duracion/2) / 29; 
  DistAlt=Distancia;
  if(Distancia <=DistMaxAlt){
    digitalWrite(LDAzAlt,HIGH);}
  else{
    digitalWrite(LDAzAlt,LOW);}
  }

//ULTRASONICO OXIMETRO
void Ultrasonico_Oximetro(){                   
  long Duracion, Distancia; 
  digitalWrite(UltOxiTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(UltOxiTrig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(UltOxiTrig, LOW);
  Duracion = pulseIn(UltOxiEcho, HIGH);
  Distancia = (Duracion/2) / 29; 
  DistOxi=Distancia;
  if(Distancia <=DistMaxOxi){
    digitalWrite(LDAzOxi,HIGH);}
  else{
    digitalWrite(LDAzOxi,LOW);}
  }
  
//DISPLAY TEMPERATURA
void Vizualizar_Temperatura(float Dato){          
  uint8_t  DPP1;
  int DP1=(int(Dato)%100)/10; 
  int DP2=int(Dato)%10;
  int DP3=int(Dato/0.1)%1000;
  int DP4=int(Dato/0.01)%10000;
  DPP1 = 0x80 | DispTemperatura.encodeDigit(DP2);
  DispTemperatura.showNumberDec(DP1,false,1,0);
  DispTemperatura.setSegments(&DPP1, 1, 1);
  DispTemperatura.showNumberDec(DP3,false,1,2);
  DispTemperatura.showNumberDec(DP4,false,1,3); 
}

//DISPLAY ALTURA
void Vizualizar_Altura(float Dato){          
  uint8_t  DPP1;
  int DP1=(int(Dato)%100)/10; 
  int DP2=int(Dato)%10;
  int DP3=int(Dato/0.1)%1000;
  int DP4=int(Dato/0.01)%10000;
  DPP1 = 0x80 | DispAltura.encodeDigit(DP2);
  DispAltura.showNumberDec(DP1,false,1,0);
  DispAltura.setSegments(&DPP1, 1, 1);
  DispAltura.showNumberDec(DP3,false,1,2);
  DispAltura.showNumberDec(DP4,false,1,3); 
}

//DISPLAY BASCULA
void Vizualizar_Bascula(float Dato){          
  uint8_t  DPP1;
  int DP1=(int(Dato)%100)/10; 
  int DP2=int(Dato)%10;
  int DP3=int(Dato/0.1)%1000;
  int DP4=int(Dato/0.01)%10000;
  DPP1 = 0x80 | DispBascula.encodeDigit(DP2);
  DispBascula.showNumberDec(DP1,false,1,0);
  DispBascula.setSegments(&DPP1, 1, 1);
  DispBascula.showNumberDec(DP3,false,1,2);
  DispBascula.showNumberDec(DP4,false,1,3); 
}

//DISPLAY OXIMETRO
void Vizualizar_Oximetro(float Dato){          
  uint8_t  DPP1;
  int DP1=(int(Dato)%100)/10; 
  int DP2=int(Dato)%10;
  int DP3=int(Dato/0.1)%1000;
  int DP4=int(Dato/0.01)%10000;
  DPP1 = 0x80 | DispOximetro.encodeDigit(DP2);
  DispOximetro.showNumberDec(DP1,false,1,0);
  DispOximetro.setSegments(&DPP1, 1, 1);
  DispOximetro.showNumberDec(DP3,false,1,2);
  DispOximetro.showNumberDec(DP4,false,1,3); 
}

//DISPLAY OXIMETRO
void Vizualizar_IMC(float Dato){        
  uint8_t  DPP1;
  int DP1=(int(Dato)%100)/10; 
  int DP2=int(Dato)%10;
  int DP3=int(Dato/0.1)%1000;
  int DP4=int(Dato/0.01)%10000;
  DPP1 = 0x80 | DispIMC.encodeDigit(DP2);
  DispIMC.showNumberDec(DP1,false,1,0);
  DispIMC.setSegments(&DPP1, 1, 1);
  DispIMC.showNumberDec(DP3,false,1,2);
  DispIMC.showNumberDec(DP4,false,1,3); 
}

void Buzzer_Temperatura(){   
  switch (EstTemperatura){
    case 0:
      if(TCambTemp>=200){
        UlTCambTemp=Tiempo;
        digitalWrite(BuzzTemp, HIGH);
        EstTemperatura=1;}
    break;  
    case 1:
      if(TCambTemp>=300){
        UlTCambTemp=Tiempo;
        digitalWrite(BuzzTemp, LOW);
         EstTemperatura=0;}
    break; }
}

//BUZER IOMT
void Buzzer_IOMT(){   
  switch (Estado6){
    case 0:
      if(TiempoCambio6>=800){
        UltimoCambio6=Tiempo;
        digitalWrite(BuzzOxi, LOW);
        Estado6=1;}
    break;  
    case 1:
      if(TiempoCambio6>=80){
        UltimoCambio6=Tiempo;
        digitalWrite(BuzzOxi, HIGH);
         Estado6=0;}
    break;}
}

//INICIO Y CALIBRACION BASCULA
void Inicio_Bascula(){
  Bascula.read();
  Bascula.read_average(20); 
  Bascula.get_value(5);  
  Bascula.get_units(5);  
  Bascula.set_scale(11072.45); //PARAMETRO PARA ESCALAR
  Bascula.tare();  
  Bascula.read(); 
  Bascula.read_average(20); 
  Bascula.get_value(5);  
  Bascula.get_units(5);
  digitalWrite(BuzzBas, HIGH);
  delay(500);
  digitalWrite(BuzzBas, LOW);
  }

void Inicio_Altura(){
  long Duracion, Distancia; 
  digitalWrite(UltAltTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(UltAltTrig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(UltAltTrig, LOW);
  Duracion = pulseIn(UltAltEcho, HIGH);
  Distancia = (Duracion/2) / 29;   
  }
  
//CONDICIONES DE LOS PINES ENTRADAS/SALIDAS
void Condicion_Pines(){
  pinMode(UltTempTrig, OUTPUT); //TEMPERATURA
  pinMode(UltTempEcho, INPUT);
  pinMode(LDVeTemp ,   OUTPUT);
  pinMode(LDAzTemp ,   OUTPUT);
  pinMode(ReleRGBTemV, OUTPUT);
  pinMode(ReleRGBTemR, OUTPUT);
  pinMode(BuzzTemp    , OUTPUT);

  pinMode(UltBasTrig, OUTPUT); //BASCULA
  pinMode(UltBasEcho, INPUT);
  pinMode(LDVeBas , OUTPUT);
  pinMode(LDVeIMC , OUTPUT);
  pinMode(BuzzBas , OUTPUT);
  
  pinMode(UltAltTrig, OUTPUT); //ALTURA
  pinMode(UltAltEcho, INPUT);
  pinMode(LDAzAlt , OUTPUT);
  pinMode(LDVeAlt , OUTPUT);

  pinMode(UltOxiTrig, OUTPUT); //OXIMETRO
  pinMode(UltOxiEcho, INPUT);  
  pinMode(LDAzOxi , OUTPUT);
  pinMode(LDVeOxi , OUTPUT);  
  pinMode(BuzzOxi , OUTPUT);
  pinMode(ActivaOxi , OUTPUT);
}
  
//ESTADO INICIAL DE CADA PIN
void Inicio_Parametros(){
  //TEMPERATURA
  digitalWrite(LDVeTemp,LOW);
  digitalWrite(LDAzTemp,LOW);
  digitalWrite(ReleRGBTemV,HIGH);
  digitalWrite(ReleRGBTemR ,HIGH);
  digitalWrite(BuzzTemp,LOW);
  //BASCULA
  digitalWrite(LDVeBas,LOW);
  digitalWrite(LDVeIMC,LOW);
  digitalWrite(BuzzBas,LOW);
  //ALTURA
  digitalWrite(LDAzAlt , LOW);
  digitalWrite(LDVeAlt , LOW);
  //OXIMETRO
  digitalWrite(LDAzOxi , LOW);
  digitalWrite(LDVeOxi , LOW);
  digitalWrite(BuzzOxi,LOW);
  digitalWrite(ActivaOxi , LOW);
  DISP_BIENVENIDA();
}

void DISP_CERO(){
  DispTemperatura.showNumberDec(0,false);
  DispAltura.showNumberDec(0,false);
  DispBascula.showNumberDec(0,false);
  DispOximetro.showNumberDec(0,false);
  DispIMC.showNumberDec(0,false);
  }

//INDICADOR DE QUE LA CABINA IOMT ESTA RECOLECTANDO DATOS
void Leer() {
  if (Serial.available()>0) {
    char c = Serial.read();
    if (c == 'O') {
      Buzzer_IOMT(); }
    else{
      digitalWrite(BuzzOxi, LOW); } }
  else{
   digitalWrite(BuzzOxi, LOW); }
}

 //MENSAJE DE BIENVENIDA DE LOS DISP TM1637 
void DISP_BIENVENIDA(){
  const uint8_t HOLA[]={
  SEG_B|SEG_C|SEG_E|SEG_F|SEG_G,
  SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F,
  SEG_D|SEG_E|SEG_F,
  SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G};
  
  const uint8_t U[]={SEG_A|SEG_B|SEG_D|SEG_E|SEG_G, SEG_D,SEG_D,SEG_B|SEG_C|SEG_D|SEG_E|SEG_F};
  const uint8_t N[]={SEG_A|SEG_B|SEG_D|SEG_E|SEG_G, SEG_D,SEG_D,SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G};
  const uint8_t A[]={SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F, SEG_D,SEG_D,SEG_A|SEG_B|SEG_C|SEG_E|SEG_F};
  const uint8_t C[]={SEG_A|SEG_B|SEG_D|SEG_E|SEG_G, SEG_D,SEG_D,SEG_A|SEG_E|SEG_F|SEG_D};
  const uint8_t H[]={SEG_D, SEG_D,SEG_D,SEG_B|SEG_C|SEG_E|SEG_F|SEG_G};
  
  DispTemperatura.setSegments(U,4,0);
  DispAltura.setSegments(N,4,0);
  DispBascula.setSegments(A,4,0);
  DispOximetro.setSegments(C,4,0);
  DispIMC.setSegments(H,4,0);
  delay(500);
  
  DispTemperatura.setSegments(HOLA,4,0);
  DispAltura.setSegments(HOLA,4,0);
  DispBascula.setSegments(HOLA,4,0);
  DispOximetro.setSegments(HOLA,4,0);
  DispIMC.setSegments(HOLA,4,0);
  delay(1000);
  DISP_CERO();
  }

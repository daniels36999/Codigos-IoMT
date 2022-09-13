const int trigPin = 8;
const int echoPin = 9;

int relebomba=3;
long duracion;
int distancia;

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relebomba, OUTPUT);
  digitalWrite(relebomba , HIGH);
  delay(4000);
  digitalWrite(relebomba , LOW);
  Serial.begin(9600);}
  
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracion = pulseIn(echoPin, HIGH);
  distancia = (duracion * 0.034) / 2;
  Serial.println(distancia);

  if(distancia >= 26 && distancia <= 100){
  delay(500);
  digitalWrite(relebomba , HIGH);
  delay(2000);
  digitalWrite(relebomba , LOW);}
  
  else{ 
  digitalWrite(relebomba , LOW); }
}

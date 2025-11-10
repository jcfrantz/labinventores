// Programa para controlar o sensor ultrassônico HC-SR04 
// Desenvolvido para medir o relevo
// Autores: Prof. Julio Cesar Frantz e Raquel Santos
// Laboratório de Metodologias Ativas da Unifebe

#define TRIG 7
#define ECHO 6

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  float distancia_cm = duracao * 0.034 / 2;

  Serial.print("Distancia (cm): ");
  Serial.println(distancia_cm);

  delay(1000);
}

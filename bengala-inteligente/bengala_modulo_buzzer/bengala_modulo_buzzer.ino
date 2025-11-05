/*
  Projeto: Bengala Inteligente com Arduino
  Autor: Professor Julio Cesar Frantz
  Data: 19/09/2025

  Descrição:
  Este código implementa o controle de uma bengala eletrônica utilizando um sensor ultrassônico HC-SR04
  para detectar obstáculos à frente do usuário. Quando um obstáculo é detectado a uma distância inferior
  a 40 cm, um buzzer, que é um módulo com este script, é acionado com uma sequência de sons curtos, alertando o usuário.

  Este projeto é ideal para aplicações em acessibilidade e pode ser expandido com motores vibratórios,
  GPS, ou alertas por vibração. O uso de som oferece um recurso de alerta simples e direto, sendo eficaz
  para pessoas com deficiência visual ou mobilidade reduzida.

  Tecnologias utilizadas:
  - Sensor Ultrassônico HC-SR04 (para medir a distância até obstáculos)
  - Módulo Buzzer (para emitir som de alerta)
  - Arduino Uno (ou equivalente)

  Bibliotecas utilizadas:
  - Buzzer.h (para controle do buzzer com notas musicais, biblioteca Buzzer by Giuseppe Martini)
  - Ultrasonic.h (para leitura de distância com o sensor HC-SR04, biblioteca Ultrasonic by Erick Simoes)

   * ---------------------    --------------------
 * | HC-SC04 | Arduino |    | Buzzer | Arduino |
 * ---------------------    --------------------
 * |   Vcc   |   5V    |    |   VCC    |   3.3V  |
 * |   Trig  |   5     | E  |   I/O    |   7     |
 * |   Echo  |   6     |    |   GND    |   GND   |
   |   GND   |   GND   |    -------------------
 * ---------------------
 * Nota: Voce nao precisa obrigatoriamente utilizar os pinos acima
  
*/

#include <Ultrasonic.h> // Inclui a biblioteca para o sensor ultrassônico

// Define os pinos para o sensor ultrassônico
// Ultrasonic(TRIG_PIN, ECHO_PIN)
Ultrasonic ultrasonic(5, 6); // Pino TRIG no 5, Pino ECHO no 6

int buzzer = 7; // Pino onde o buzzer está conectado

void setup() {
  Serial.begin(9600);    // Inicia a comunicação serial para debug (opcional, mas recomendado)
  pinMode(buzzer, OUTPUT); // Define o pino do buzzer como saída
  digitalWrite(buzzer, LOW); // Garante que o buzzer comece desligado
}

void loop() {
  // Lê a distância em centímetros
  long distanciaCm = ultrasonic.read();

  // Imprime a distância no monitor serial para debug
  Serial.print("Distancia: ");
  Serial.print(distanciaCm);
  Serial.println(" cm");

  // Verifica se a distância é maior que 40 cm
  if (distanciaCm > 40) {
    digitalWrite(buzzer, HIGH); // Liga o buzzer
    Serial.println("Buzzer: LIGADO");
  } else {
    digitalWrite(buzzer, LOW);  // Desliga o buzzer
    Serial.println("Buzzer: DESLIGADO");
  }

  delay(100); // Pequeno delay para evitar leituras muito rápidas e estabilizar
}
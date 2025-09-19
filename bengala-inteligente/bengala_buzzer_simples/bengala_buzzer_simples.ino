/*
  Projeto: Bengala Inteligente com Arduino
  Autor: Professor Julio Cesar Frantz
  Data: 24/06/2025

  Descrição:
  Este código implementa o controle de uma bengala eletrônica utilizando um sensor ultrassônico HC-SR04
  para detectar obstáculos à frente do usuário. Quando um obstáculo é detectado a uma distância inferior
  a 40 cm, um buzzer é acionado com uma sequência de sons curtos, alertando o usuário.

  Este projeto é ideal para aplicações em acessibilidade e pode ser expandido com motores vibratórios,
  GPS, ou alertas por vibração. O uso de som oferece um recurso de alerta simples e direto, sendo eficaz
  para pessoas com deficiência visual ou mobilidade reduzida.

  Tecnologias utilizadas:
  - Sensor Ultrassônico HC-SR04 (para medir a distância até obstáculos)
  - Buzzer (para emitir som de alerta)
  - Arduino Uno (ou equivalente)

  Bibliotecas utilizadas:
  - Buzzer.h (para controle do buzzer com notas musicais, biblioteca Buzzer by Giuseppe Martini)
  - Ultrasonic.h (para leitura de distância com o sensor HC-SR04, biblioteca Ultrasonic by Erick Simoes)

   * ---------------------    --------------------
 * | HC-SC04 | Arduino |    | Buzzer | Arduino |
 * ---------------------    --------------------
 * |   Vcc   |   5V    |    |   +    |   7    |
 * |   Trig  |   5     | E  |   -    |   GND  |
 * |   Echo  |   6     |    -------------------
 * |   Gnd   |   GND   |   
 * ---------------------
 * Nota: Voce nao precisa obrigatoriamente utilizar os pinos acima
  
*/

#include <Buzzer.h>
#include <Ultrasonic.h> 

// Pinos do buzzer: controle e terra (GND)
Buzzer buzzer(7);  // pino de sinal no 7

Ultrasonic ultrasonic(5, 6);
int distance;

void setup() {
  Serial.begin(9600);  // Inicia a comunicação serial para monitoramento
}

void loop() {
  // Medição do tempo de ida e volta do sinal ultrassônico (em microssegundos)
   distance = ultrasonic.read();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);

  // Verifica se há obstáculo a menos de 40 cm
  if (distance < 40) {
    buzzer.begin(10);  // Inicia o controle do buzzer

    // Sequência sonora de alerta
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_C7, 80);
    buzzer.sound(NOTE_E7, 80);

    buzzer.end(10);    // Finaliza o controle do buzzer
  }

  delay(100);  // Pequeno atraso entre leituras
}

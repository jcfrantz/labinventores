
# Bengala Inteligente com Arduino para Detecção de Obstáculos

Este projeto implementa uma bengala eletrônica inteligente utilizando um Arduino e um sensor ultrassônico HC-SR04. O objetivo é detectar obstáculos a uma distância inferior a 40 cm e alertar o usuário por meio de um sinal sonoro emitido por um buzzer, aumentando a segurança e a mobilidade de pessoas com deficiência visual.

A versão do arquivo .ino 'bengala_buzzer_simples' utiliza apenas um buzzer ativo de 5V e a versão 'bengala_modulo_buzzer' utiliza um módulo, por isso os scripts são diferentes. Baixe de acordo com o buzzer que você irá utilizar. O esquema de conexão está declarado nos scripts.

A seguir, detalho a construção da bengala com o buzzer ativo de 5V.

[![bengala-inteligente-arduino](https://img.youtube.com/vi/UyF4Qit7c3Y/0.jpg)](https://youtu.be/UyF4Qit7c3Y)

## Funcionalidades

- **Detecção de Obstáculos**: Utiliza um sensor ultrassônico HC-SR04 para medir a distância de objetos à frente.
- **Alerta Sonoro**: Emite uma sequência de sons curtos através de um buzzer quando um obstáculo está a menos de 40 cm.
- **Monitoramento Serial**: Exibe a distância medida em centímetros no monitor serial, facilitando a calibração e testes.
- **Baixo Custo e Simplicidade**: Construído com componentes eletrônicos acessíveis e de fácil montagem.

## Componentes Utilizados

- **Arduino Uno** ou qualquer outra placa compatível
- **Sensor Ultrassônico HC-SR04**
- **Buzzer**
- **Jumpers e Protoboard**

## Esquema de Conexão

| Componente | Pino no Arduino |
| :--- | :--- |
| Sensor HC-SR04 (VCC) | 5V |
| Sensor HC-SR04 (GND) | GND |
| Sensor HC-SR04 (Trig) | 5 |
| Sensor HC-SR04 (Echo) | 6 |
| Buzzer (+) | 7 |
| Buzzer (-) | GND |

<img width="1536" height="640" alt="Bengala Automatizada - Circuito com Arduino" src="https://github.com/user-attachments/assets/b5bcb66f-0baf-40ff-9f90-5a0dfa6477d8" />

- Link para o TinkerCAD: [[Circuito no TinkerCAD]([https://www.tinkercad.com/things/hi7dyhZLCyG-bengala-automatizada-circuito-com-arduino/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits&sharecode=3s2cAmV87z2F8LSTdyxHtvA-ZVuK8qj6tw_4shxL5cc](https://www.tinkercad.com/things/hi7dyhZLCyG-bengala-automatizada-circuito-com-arduino)https://www.tinkercad.com/things/hi7dyhZLCyG-bengala-automatizada-circuito-com-arduino)](https://www.tinkercad.com/things/hi7dyhZLCyG-bengala-automatizada-circuito-com-arduino)

## Como Funciona

1. **Medição de Distância**:
   - O pino `Trig` do sensor ultrassônico emite um pulso sonoro de alta frequência.
   - O pino `Echo` aguarda o retorno do som após atingir um obstáculo.
   - O Arduino mede o tempo que o som levou para ir e voltar e calcula a distância em centímetros.

2. **Alerta de Proximidade**:
   - O código verifica continuamente a distância medida.
   - Se a distância for menor que 40 cm, a função de alerta é acionada.
   - O buzzer emite uma sequência de notas musicais para notificar o usuário sobre o obstáculo.

## Código Arduino

Aqui está o código utilizado para o projeto:

```cpp
/*
  Projeto: Bengala Inteligente com Arduino
  Autor: Professor Julio Cesar Frantz
  Data: 24/06/2025

  Descrição:
  Este código implementa o controle de uma bengala eletrônica utilizando um sensor ultrassônico HC-SR04
  para detectar obstáculos à frente do usuário. Quando um obstáculo é detectado a uma distância inferior
  a 40 cm, um buzzer é acionado com uma sequência de sons curtos, alertando o usuário.
*/

#include <Buzzer.h>
#include <Ultrasonic.h> 

// Define o pino de sinal do buzzer
Buzzer buzzer(7);

// Define os pinos Trig e Echo do sensor ultrassônico
Ultrasonic ultrasonic(5, 6);
int distance;

void setup() {
  Serial.begin(9600);  // Inicia a comunicação serial para monitoramento
}

void loop() {
  // Lê a distância do sensor em centímetros
  distance = ultrasonic.read();
  
  // Exibe a distância no monitor serial
  Serial.print("Distance in CM: ");
  Serial.println(distance);

  // Verifica se há um obstáculo a menos de 40 cm
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

  delay(100);  // Pequeno atraso entre leituras para estabilidade
}
````

## Instruções de uso

1.  **Montagem do Circuito**: Conecte todos os componentes conforme o esquema de conexão fornecido.
2.  **Instalação das Bibliotecas**:
      - Abra a IDE do Arduino.
      - Vá em `Sketch` \> `Include Library` \> `Manage Libraries...`.
      - Procure por "**Ultrasonic by Erick Simoes**" e instale.
      - Procure por "**Buzzer by Giuseppe Martini**" e instale.
3.  **Carregar o Código**: Copie o código acima e carregue-o na sua placa Arduino.
4.  **Testar o Funcionamento**:
      - Com o Arduino conectado ao computador, abra o Monitor Serial (`Tools` \> `Serial Monitor`) com a velocidade de 9600 baud.
      - Aproxime um objeto (como sua mão) do sensor ultrassônico.
      - Observe a distância impressa no monitor e ouça o buzzer quando o objeto estiver a menos de 40 cm.

<!-- end list -->

```
```

# Controle de Velocidade de Motor DC com Ponte H BTS7960

Este projeto implementa o controle de um motor DC utilizando a ponte H BTS7960 com um Arduino. O sentido de rotação do motor (horário e anti-horário) é controlado por dois botões, e a velocidade pode ser ajustada usando dois botões adicionais para aumentar ou diminuir a velocidade em incrementos de 25. Há também um botão dedicado para desligar o motor.

## Funcionalidades

- **Controle de Sentido de Rotação**: O motor pode girar no sentido horário ou anti-horário.
- **Ajuste de Velocidade**: A velocidade pode ser ajustada em incrementos de 25 (de 0 a 255) por meio de dois botões.
- **Desligamento do Motor**: Um botão adicional permite desligar o motor completamente e zerar a velocidade.
- **Monitoramento pela Saída Serial**: A rotação e a velocidade atuais são exibidas no monitor serial.

## Componentes Utilizados

- **Arduino Uno** ou qualquer outra placa compatível
- **Ponte H BTS7960**
- **Motor DC**
- **Botões (5)**
- **Jumpers e Protoboard**
- **Fonte de alimentação para o motor DC**

## Esquema de Conexão

| Componente      | Pino no Arduino  |
|-----------------|------------------|
| Botão Sentido Anti-Horário (Botão 1) | 6 |
| Botão Sentido Horário (Botão 2)      | 7 |
| Botão Desligar Motor (Botão 3)       | 8 |
| Botão Aumentar Velocidade (Botão 4)  | 5 |
| Botão Diminuir Velocidade (Botão 5)  | 4 |
| PWM Sentido Horário                  | 10 |
| PWM Sentido Anti-Horário             | 9 |
| Habilitar Sentido Horário            | 16 |
| Habilitar Sentido Anti-Horário       | 14 |

## Como Funciona

1. **Sentido de Rotação**:
   - Pressione o **botão 1** para girar o motor no sentido anti-horário.
   - Pressione o **botão 2** para girar o motor no sentido horário.

2. **Ajuste de Velocidade**:
   - Pressione o **botão 4** para aumentar a velocidade em incrementos de 25.
   - Pressione o **botão 5** para diminuir a velocidade em decrementos de 25.

3. **Desligamento**:
   - Pressione o **botão 3** para desligar o motor e zerar a velocidade.

## Código Arduino

Aqui está o código utilizado para controlar o motor:

```cpp
// Definições de pinos
const int BUTTON_PIN1 = 6;       // o número do pino do botão 1 (sentido anti-horário)
const int BUTTON_PIN2 = 7;       // o número do pino do botão 2 (sentido horário)
const int BUTTON_PIN3 = 8;       // o número do pino do botão 3 (desligar)
const int BUTTON_PIN4 = 5;       // o número do pino do botão 4 (aumentar velocidade)
const int BUTTON_PIN5 = 4;       // o número do pino do botão 5 (diminuir velocidade)
const int DEBOUNCE_DELAY = 50;   // o tempo de debounce; aumente se a saída piscar
const int rpwmPin = 10;          // PWM para sentido horário
const int lpwmPin = 9;           // PWM para sentido anti-horário
const int renPin = 16;           // Habilita sentido horário
const int lenPin = 14;           // Habilita sentido anti-horário
const int SPEED_STEP = 25;       // Incremento de velocidade (0 a 255)

int motorSpeed = 0;              // Velocidade inicial do motor

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
  pinMode(BUTTON_PIN4, INPUT_PULLUP);
  pinMode(BUTTON_PIN5, INPUT_PULLUP);
  pinMode(rpwmPin, OUTPUT);
  pinMode(lpwmPin, OUTPUT);
  pinMode(renPin, OUTPUT);
  pinMode(lenPin, OUTPUT);
  Serial.println("Sistema Inicializado");
}

void loop() {
  // Funções para controle do motor
  controlarSentido();
  ajustarVelocidade();
  desligarMotor();
}

void controlarSentido() {
  if (digitalRead(BUTTON_PIN1) == LOW) {  // Sentido anti-horário
    digitalWrite(renPin, HIGH);
    digitalWrite(lenPin, HIGH);
    analogWrite(rpwmPin, 0);
    analogWrite(lpwmPin, motorSpeed);
    Serial.println("Motor Girando Anti-Horário");
  } else if (digitalRead(BUTTON_PIN2) == LOW) {  // Sentido horário
    digitalWrite(renPin, HIGH);
    digitalWrite(lenPin, HIGH);
    analogWrite(rpwmPin, motorSpeed);
    analogWrite(lpwmPin, 0);
    Serial.println("Motor Girando Horário");
  }
}

void ajustarVelocidade() {
  if (digitalRead(BUTTON_PIN4) == LOW && motorSpeed < 255) {  // Aumenta a velocidade
    motorSpeed += SPEED_STEP;
    if (motorSpeed > 255) motorSpeed = 255;
    Serial.print("Velocidade Aumentada: ");
    Serial.println(motorSpeed);
    delay(250);
  }

  if (digitalRead(BUTTON_PIN5) == LOW && motorSpeed > 0) {  // Diminui a velocidade
    motorSpeed -= SPEED_STEP;
    if (motorSpeed < 0) motorSpeed = 0;
    Serial.print("Velocidade Diminuída: ");
    Serial.println(motorSpeed);
    delay(250);
  }
}

void desligarMotor() {
  if (digitalRead(BUTTON_PIN3) == LOW) {  // Desliga o motor
    motorSpeed = 0;
    analogWrite(rpwmPin, 0);
    analogWrite(lpwmPin, 0);
    Serial.println("Motor Desligado");
  }
}

## Instruções de uso

1. Conecte todos os componentes conforme o esquema de conexão fornecido.
2. Carregue o código no Arduino usando a IDE do Arduino.
3. Abra o monitor serial para monitorar o sentido de rotação e a velocidade.
4. Utilize os botões para controlar a direção e velocidade do motor conforme explicado acima.

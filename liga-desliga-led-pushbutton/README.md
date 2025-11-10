# Controle de LED/Motor com Arduino e Debounce

Este projeto utiliza dois botões para ligar e desligar um LED (ou motor) usando um Arduino. O programa implementa a técnica de **debounce**, que garante leituras estáveis dos botões, prevenindo leituras incorretas devido a ruídos elétricos ou flutuações rápidas.

![Vídeo sem título ‐ Feito com o Clipchamp](https://github.com/user-attachments/assets/30a03102-e0f4-49d6-9734-3fda35f0ca8c)

## Funcionalidades

- **Botão 1 (pino 5)**: Liga o LED/motor.
- **Botão 2 (pino 6)**: Desliga o LED/motor.
- **Debounce**: Utiliza um temporizador para garantir que o estado do botão só seja alterado após uma leitura estável.

## Componentes Utilizados

- **Arduino Uno** (ou qualquer placa Arduino compatível)
- **2 Pushbuttons**
- **1 LED ou Motor**
- **1 resistor 220 ohms para o LED** 
- **Protoboard e Jumpers**

## Esquema de Conexão

| Componente  | Pino no Arduino |
|-------------|-----------------|
| Botão 1     | 5               |
| Botão 2     | 6               |
| LED/Motor   | 8               |

![Fantastic Turing](https://github.com/user-attachments/assets/33d20bf7-ca8e-4fb0-afd7-c2144bd2c921)

- Link para o TinkerCAD: https://www.tinkercad.com/things/inf8xFcbAwx-debounce-arduino
- Conecte um terminal de cada botão ao pino correspondente e o outro terminal ao **GND**.
- O LED ou motor deve estar conectado ao **pino 8** e ao **GND**.

## Como Funciona

- Pressionar o **Botão 1** liga o LED/motor.
- Pressionar o **Botão 2** desliga o LED/motor.
- O código usa uma função de debounce para evitar leituras incorretas e garantir que o botão esteja pressionado por um tempo mínimo antes de alterar o estado do LED/motor.

## Código Arduino

```cpp
// Definindo os pinos do LED e dos pushbuttons
const int ledPin = 8;        // Pino onde o LED está conectado
const int buttonPin1 = 5;     // Pino do botão para ligar o LED
const int buttonPin2 = 6;     // Pino do botão para desligar o LED

// Variáveis para armazenar o estado dos botões
int buttonState1 = HIGH;      // Estado atual do botão 1
int lastButtonState1 = HIGH;  // Último estado do botão 1
int buttonState2 = HIGH;      // Estado atual do botão 2
int lastButtonState2 = HIGH;  // Último estado do botão 2

// Variáveis para debounce
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 50; // Tempo de debounce em milissegundos

// Estado do LED (inicia desligado)
bool ledState = LOW;

void setup() {
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState); // Inicializa o LED desligado

  // Configura os pinos dos botões como entradas com pull-up
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
}

void loop() {
  // Lê o estado atual dos botões
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);

  // Verifica se houve mudança no estado do botão 1 (ligar)
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis(); // Reinicia o tempo de debounce
  }

  // Verifica se houve mudança no estado do botão 2 (desligar)
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis(); // Reinicia o tempo de debounce
  }

  // Lida com debounce para o botão 1
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (lastButtonState1 == HIGH && reading1 == LOW) {
      ledState = HIGH; // Liga o LED
    }
    lastButtonState1 = reading1;
  }

  // Lida com debounce para o botão 2
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (lastButtonState2 == HIGH && reading2 == LOW) {
      ledState = LOW; // Desliga o LED
    }
    lastButtonState2 = reading2;
  }

  // Atualiza o estado do LED
  digitalWrite(ledPin, ledState);
}

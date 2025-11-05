/*
  Programa para controle de motor DC utilizando Ponte-H BTS7960
  Autor: Julio Cesar Frantz
  Projeto: Controle de direção e velocidade de motor com Arduino
  Contato: julio.frantz@labinventores.com.br
  Tutorial completo: [Link para o tutorial]

  Descrição: 
  Este programa controla a rotação de um motor DC utilizando uma ponte H BTS7960.
  O sentido de rotação (horário ou anti-horário) é controlado por dois botões, 
  e a velocidade é ajustada por dois botões adicionais para aumentar ou diminuir 
  a velocidade em incrementos de 25. Também há um botão para desligar o motor.
  
  Pinos utilizados:
  - BUTTON_PIN1: Sentido anti-horário
  - BUTTON_PIN2: Sentido horário
  - BUTTON_PIN3: Desligar motor
  - BUTTON_PIN4: Aumentar velocidade
  - BUTTON_PIN5: Diminuir velocidade
  - rpwmPin: PWM para sentido horário
  - lpwmPin: PWM para sentido anti-horário
  - renPin: Habilita sentido horário
  - lenPin: Habilita sentido anti-horário
*/

// constantes usadas aqui para definir números de pinos:
const int BUTTON_PIN1 = 6;       // o número do pino do botão 1 (sentido anti-horário)
const int BUTTON_PIN2 = 7;       // o número do pino do botão 2 (sentido horário)
const int BUTTON_PIN3 = 8;       // o número do pino do botão 3 (desligar)
const int BUTTON_PIN4 = 5;       // o número do pino do botão 4 (aumentar velocidade)
const int BUTTON_PIN5 = 4;      // o número do pino do botão 5 (diminuir velocidade)
const int DEBOUNCE_DELAY = 50;   // o tempo de debounce; aumente se a saída piscar
const int rpwmPin = 10;          // PWM para sentido horário
const int lpwmPin = 9;          // PWM para sentido anti-horário
const int renPin = 16;           // Habilita sentido horário
const int lenPin = 14;           // Habilita sentido anti-horário
const int SPEED_STEP = 25;       // Incremento de velocidade (0 a 255)

// Variáveis que mudarão:
unsigned long lastDebounceTime1 = 0;  // última vez que o pino de saída foi alternado para o botão 1
unsigned long lastDebounceTime2 = 0;  // última vez que o pino de saída foi alternado para o botão 2
unsigned long lastDebounceTime3 = 0;  // última vez que o pino de saída foi alternado para o botão 3
unsigned long lastDebounceTime4 = 0;  // última vez que o pino de saída foi alternado para o botão 4 (aumentar velocidade)
unsigned long lastDebounceTime5 = 0;  // última vez que o pino de saída foi alternado para o botão 5 (diminuir velocidade)

bool motorHorario = false;            // Armazena o estado do motor no sentido horário
bool motorAntiHorario = false;        // Armazena o estado do motor no sentido anti-horário
bool motorDesligado = false;          // Armazena o estado de desligamento do motor
int motorSpeed = 0;                   // Velocidade inicial do motor (0 a 255)

// Variáveis para controlar o incremento único de velocidade
bool lastStateButton4 = HIGH;         // Armazena o último estado do botão 4 (aumentar velocidade)
bool lastStateButton5 = HIGH;         // Armazena o último estado do botão 5 (diminuir velocidade)

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);  // Botão para selecionar sentido anti-horário
  pinMode(BUTTON_PIN2, INPUT_PULLUP);  // Botão para selecionar sentido horário
  pinMode(BUTTON_PIN3, INPUT_PULLUP);  // Botão para desligar o motor
  pinMode(BUTTON_PIN4, INPUT_PULLUP);  // Botão para aumentar a velocidade
  pinMode(BUTTON_PIN5, INPUT_PULLUP);  // Botão para diminuir a velocidade
  pinMode(rpwmPin, OUTPUT);
  pinMode(lpwmPin, OUTPUT);
  pinMode(renPin, OUTPUT);
  pinMode(lenPin, OUTPUT);
  Serial.println("Sistema Inicializado");
}

void loop() {
  botaoLigaHorario();
  botaoLigaAntiHorario();
  botaoDesligaMotor();
  botaoAumentaVelocidade();
  botaoDiminuiVelocidade();
  atualizaMotor();
}

void botaoLigaAntiHorario() {
  static int lastState1 = HIGH; // Armazena o último estado do botão 1
  int currentState1 = digitalRead(BUTTON_PIN1);

  // Se o botão mudou de estado (de HIGH para LOW)
  if (currentState1 != lastState1) {
    lastDebounceTime1 = millis(); // reinicia o tempo de debounce
  }

  if ((millis() - lastDebounceTime1) > DEBOUNCE_DELAY) {
    if (currentState1 == LOW) { // O botão foi pressionado
      motorAntiHorario = true;   // Liga o motor no sentido anti-horário
      motorHorario = false;      // Garante que o sentido horário está desligado
      motorDesligado = false;    // O motor não está desligado
      Serial.println("Motor Ligado Anti-Horário");
    }
  }

  lastState1 = currentState1; // atualiza o último estado do botão 1
}

void botaoLigaHorario() {
  static int lastState2 = HIGH; // Armazena o último estado do botão 2
  int currentState2 = digitalRead(BUTTON_PIN2);

  // Se o botão mudou de estado (de HIGH para LOW)
  if (currentState2 != lastState2) {
    lastDebounceTime2 = millis(); // reinicia o tempo de debounce
  }

  if ((millis() - lastDebounceTime2) > DEBOUNCE_DELAY) {
    if (currentState2 == LOW) { // O botão foi pressionado
      motorHorario = true;       // Liga o motor no sentido horário
      motorAntiHorario = false;  // Garante que o sentido anti-horário está desligado
      motorDesligado = false;    // O motor não está desligado
      Serial.println("Motor Ligado Horário");
    }
  }

  lastState2 = currentState2; // atualiza o último estado do botão 2
}

void botaoDesligaMotor() {
  static int lastState3 = HIGH; // Armazena o último estado do botão 3
  int currentState3 = digitalRead(BUTTON_PIN3);

  // Se o botão de desligar mudou de estado (de HIGH para LOW)
  if (currentState3 != lastState3) {
    lastDebounceTime3 = millis(); // reinicia o tempo de debounce
  }

  if ((millis() - lastDebounceTime3) > DEBOUNCE_DELAY) {
    if (currentState3 == LOW) { // O botão de desligar foi pressionado
      motorHorario = false;      // Desliga o motor no sentido horário
      motorAntiHorario = false;  // Desliga o motor no sentido anti-horário
      motorDesligado = true;     // Define que o motor foi desligado
      motorSpeed = 0;            // Redefine a velocidade para zero
      Serial.println("Motor Desligado");
    }
  }

  lastState3 = currentState3; // atualiza o último estado do botão 3
}

void botaoAumentaVelocidade() {
  int currentStateButton4 = digitalRead(BUTTON_PIN4);

  // Detecta quando o botão é pressionado (mudança de HIGH para LOW)
  if (currentStateButton4 == LOW && lastStateButton4 == HIGH) {
    if (motorSpeed + SPEED_STEP <= 255) {
      motorSpeed += SPEED_STEP;  // Aumenta a velocidade em 25
    } else {
      motorSpeed = 255;          // Limita a velocidade máxima
    }
    Serial.print("Velocidade Aumentada: ");
    Serial.println(motorSpeed);
  }

  // Atualiza o estado anterior do botão
  lastStateButton4 = currentStateButton4;
}

void botaoDiminuiVelocidade() {
  int currentStateButton5 = digitalRead(BUTTON_PIN5);

  // Detecta quando o botão é pressionado (mudança de HIGH para LOW)
  if (currentStateButton5 == LOW && lastStateButton5 == HIGH) {
    if (motorSpeed - SPEED_STEP >= 0) {
      motorSpeed -= SPEED_STEP;  // Diminui a velocidade em 25
    } else {
      motorSpeed = 0;            // Limita a velocidade mínima
    }
    Serial.print("Velocidade Diminuída: ");
    Serial.println(motorSpeed);
  }

  // Atualiza o estado anterior do botão
  lastStateButton5 = currentStateButton5;
}

void atualizaMotor() {
  // Atualiza o estado do motor de acordo com as variáveis de sentido e desligamento
  if (motorDesligado) {
    // Se o botão de desligar foi pressionado, desliga o motor completamente
    digitalWrite(renPin, LOW);
    digitalWrite(lenPin, LOW);
    analogWrite(rpwmPin, 0);
    analogWrite(lpwmPin, 0);
  } else if (motorHorario) {
    // Se o motor está no sentido horário
    digitalWrite(lenPin, HIGH);  // Habilita direção
    digitalWrite(renPin, HIGH);
    analogWrite(lpwmPin, motorSpeed); // Sentido horário com velocidade variável
    analogWrite(rpwmPin, 0);
  } else if (motorAntiHorario) {
    // Se o motor está no sentido anti-horário
    digitalWrite(renPin, HIGH);  // Habilita direção
    digitalWrite(lenPin, HIGH);
    analogWrite(rpwmPin, motorSpeed); // Sentido anti-horário com velocidade variável
    analogWrite(lpwmPin, 0);
  }
}

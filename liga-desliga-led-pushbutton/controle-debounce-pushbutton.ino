/*
  Projeto: Controle de LED com Arduino e Debounce
  Descrição: Este programa utiliza dois botões para ligar e desligar um LED (ou motor) com controle de debounce, 
  garantindo que a leitura dos botões não seja afetada por ruídos. O primeiro botão liga o LED/motor, enquanto o 
  segundo botão desliga o LED/motor. Utiliza-se o conceito de debounce para estabilizar as leituras de pressões rápidas.

  Componentes:
  - Arduino (Uno, Mega, etc.)
  - 2 Pushbuttons
  - 1 LED ou Motor (controlado por um pino digital)
  - Resistores (se necessário, dependendo do botão)
  - Jumpers e Protoboard
  
  Pinos:
  - BUTTON_PIN1: Pino 5 - Botão de ligar
  - BUTTON_PIN2: Pino 6 - Botão de desligar
  - LED: Pino 8 - LED ou Motor controlado

  Como funciona:
  - Pressionar o botão 1 (conectado ao pino 5) liga o LED/motor.
  - Pressionar o botão 2 (conectado ao pino 6) desliga o LED/motor.
  - O debounce garante que o botão precisa ser pressionado por um tempo mínimo para considerar o estado alterado.

  Autor: Julio Cesar Frantz
  Tutorial completo: [Link para o tutorial]
  Contato: julio.frantz@labinventores.com.br
  Tutorial completo: [Link para o tutorial]
*/

// constantes não mudarão. Elas são usadas aqui para definir os números dos pinos:
const int BUTTON_PIN1 = 5;       // o número do pino do pushbutton 1
const int BUTTON_PIN2 = 6;       // o número do pino do pushbutton 2 
const int DEBOUNCE_DELAY = 50;   // o tempo de debounce; aumente se a saída piscar
const int DEBOUNCE_DELAY2 = 50;
const int LED = 8;

// As variáveis irão mudar:
int lastSteadyState1 = LOW;       // o estado estável anterior do pino de entrada
int lastFlickerableState1 = LOW;  // o estado instável anterior do pino de entrada
int currentState1;                // a leitura atual do pino de entrada

int lastSteadyState2 = LOW;       // o estado estável anterior do pino de entrada
int lastFlickerableState2 = LOW;  // o estado instável anterior do pino de entrada
int currentState2; 

// as seguintes variáveis são do tipo unsigned long porque o tempo, medido em
// milissegundos, rapidamente se tornará um número maior do que pode ser armazenado em um int.
unsigned long lastDebounceTime = 0;   // a última vez que o pino de saída foi alternado
unsigned long lastDebounceTime2 = 0;  // a última vez que o pino de saída foi alternado

void setup() {
  // inicializa a comunicação serial em 9600 bits por segundo:
  Serial.begin(9600);
  // inicializa o pino do pushbutton como uma entrada com pull-up
  // o pino de entrada com pull-up ficará em HIGH quando o interruptor estiver aberto e em LOW quando estiver fechado.
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  Serial.println("Sistema Inicializado");
}

void loop() {
  // lê o estado do interruptor/botão:
  botaoLiga();
  botaoDesliga();
}

void botaoLiga() {
  
  currentState1 = digitalRead(BUTTON_PIN1);

  // verifica se o botão acabou de ser pressionado
  // (ou seja, a entrada foi de LOW para HIGH) e se você esperou tempo suficiente
  // desde a última pressão para ignorar qualquer ruído:

  // Se o interruptor/botão mudou, devido ao ruído ou à pressão:
  if (currentState1 != lastFlickerableState1) {
    // reinicia o temporizador de debounce
    lastDebounceTime = millis();
    // salva o último estado instável
    lastFlickerableState1 = currentState1;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // qualquer que seja a leitura, ela está lá há mais tempo que o atraso de debounce,
    // então aceite isso como o estado atual real:

    // se o estado do botão mudou:
    if (lastSteadyState1 == HIGH && currentState1 == LOW) {
      Serial.println("Motor Ligado");
      digitalWrite(LED, HIGH);
    }
    // else if (lastSteadyState == LOW && currentState == HIGH) {
     // Serial.println("Motor Desligado");
     // digitalWrite(LED, LOW);
    //}

    // salva o último estado estável
    lastSteadyState1 = currentState1;
  }

}

void botaoDesliga() {
  
  currentState2 = digitalRead(BUTTON_PIN2);

  // verifica se o botão acabou de ser pressionado
  // (ou seja, a entrada foi de LOW para HIGH) e se você esperou tempo suficiente
  // desde a última pressão para ignorar qualquer ruído:

  // Se o interruptor/botão mudou, devido ao ruído ou à pressão:
  if (currentState2 != lastFlickerableState2) {
    // reinicia o temporizador de debounce
    lastDebounceTime = millis();
    // salva o último estado instável
    lastFlickerableState2 = currentState2;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // qualquer que seja a leitura, ela está lá há mais tempo que o atraso de debounce,
    // então aceite isso como o estado atual real:

    // se o estado do botão mudou:
    if (lastSteadyState2 == HIGH && currentState2 == LOW) {
      Serial.println("Motor Desligado");
      digitalWrite(LED, LOW);
    }

    // salva o último estado estável
    lastSteadyState2 = currentState2;
  }
  
}

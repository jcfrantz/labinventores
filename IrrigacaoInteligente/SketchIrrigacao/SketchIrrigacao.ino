/*
  Projeto: Irrigador Automático com Display LCD I2C
  Autor: Professor Julio Cesar Frantz
  Data: 03/11/2025 (Refatorado para I2C e padrão de comentários)

  Descrição:
  Este código implementa um sistema automatizado de irrigação. Ele utiliza um sensor
  de umidade do solo (higrômetro) para monitorar as condições da terra em tempo real.
  O valor lido é convertido para porcentagem e exibido em um display LCD I2C 16x2.

  Um módulo relé é acionado para ligar uma bomba d'água sempre que a umidade
  cair abaixo de um nível pré-determinado (48%), desligando-a quando o solo
  atinge um nível moderado ou ideal.

  Este projeto é ideal para automação residencial (vasos, hortas caseiras) ou
  projetos de agricultura de precisão em pequena escala.

  Tecnologias utilizadas:
  - Sensor de Umidade do Solo (Higrômetro, pino analógico)
  - Módulo Relé de 1 Canal (para acionar a bomba d'água)
  - Display LCD I2C 16x2 (para feedback visual)
  - Arduino Uno (ou equivalente)

  Bibliotecas utilizadas:
  - Wire.h (Biblioteca nativa do Arduino para comunicação I2C)
  - LiquidCrystal_I2C.h by Martin (Biblioteca para controle do display LCD via barramento I2C)

  ---------------------------------   -----------------------------------
| Sensor Umidade | Arduino |         | Módulo Relé   | Arduino   |
  ---------------------------------   -----------------------------------
|      VCC       |   5V    |         |     VCC       |    5V     |
|      GND       |   GND   |         |     GND       |    GND    |
|      A0        |   A0    |         |      IN       |    2      |
  ---------------------------------   -----------------------------------
|   LCD I2C      | Arduino |
  ---------------------------------
|      VCC       |   5V    |
|      GND       |   GND   |
|      SDA       |   A4    | (SDA)
|      SCL       |   A5    | (SCL)
  ---------------------------------
  Nota: Os pinos A4 e A5 são os pinos I2C padrão do Arduino Uno/Nano.
*/

#include <Wire.h>             // Biblioteca necessária para a comunicação I2C
#include <LiquidCrystal_I2C.h> // Biblioteca para o controle do display LCD

// --- Definição de Pinos ---
#define pino_sensor A0   // Pino analógico onde o sensor de umidade está conectado
#define rele_pin 2       // Pino digital que controla o módulo relé (bomba)

// --- Configuração do LCD I2C ---
// Inicializa o LCD no endereço 0x27 (pode ser 0x3F) com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Variáveis Globais ---
int valor_analogico = 0; // Armazena a leitura bruta do sensor (0-1023)
int umidade = 0;         // Armazena o valor convertido para porcentagem (0-100%)

// ==== CONFIGURAÇÃO INICIAL (SETUP) ====
void setup() {
  // Inicia a comunicação serial para fins de debug (Monitor Serial)
  Serial.begin(9600);

  // Configura os pinos
  pinMode(pino_sensor, INPUT);  // Pino do sensor é uma ENTRADA de dados
  pinMode(rele_pin, OUTPUT); // Pino do relé é uma SAÍDA de controle

  // --- Inicialização do Relé ---
  // Garante que o relé (e a bomba) comece DESLIGADO (LOW)
  digitalWrite(rele_pin, LOW);

  // --- Inicialização do LCD I2C ---
  lcd.init();      // Inicializa o objeto do display
  lcd.backlight(); // Liga a luz de fundo do display

  // Exibe uma mensagem de inicialização no LCD
  lcd.setCursor(0, 0); // Coluna 0, Linha 0
  lcd.print("SISTEMA ATIVO");
  delay(2000); // Aguarda 2 segundos
  lcd.clear(); // Limpa o display para as leituras
}

// ==== LOOP PRINCIPAL ====
void loop() {
  // --- 1. Leitura e Conversão do Sensor ---
  
  // Lê o valor analógico bruto do sensor
  valor_analogico = analogRead(pino_sensor);

  // Converte a faixa de leitura (0-1023) para porcentagem (0-100%)
  // Nota: O sensor higrômetro é "invertido":
  // 1023 = Ar/Seco (baixa condutividade)
  // 0    = Água/Úmido (alta condutividade)
  // Por isso, mapeamos 1023 para 0% e 0 para 100%.
  umidade = map(valor_analogico, 1023, 0, 0, 100);

  // --- 2. Exibição de Dados (Serial e LCD) ---

  // Envia os dados para o Monitor Serial (debug)
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println("%");

  // Prepara o LCD para exibir os dados atualizados
  lcd.clear(); // Limpa a tela a cada ciclo
  lcd.setCursor(0, 0); // Põe o cursor na Linha 0, Coluna 0
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.print("%");

  // --- 3. Lógica de Controle do Relé (Bomba) ---

  // Verifica as condições de umidade para tomar uma ação
  if (umidade < 48) {
    // CONDIÇÃO 1: Solo está seco
    digitalWrite(rele_pin, HIGH); // Aciona o relé (LIGA a bomba)
    lcd.setCursor(0, 1);          // Põe o cursor na Linha 1
    lcd.print("SOLO SECO-REGAR");
  }
  else if (umidade >= 48 && umidade <= 64) {
    // CONDIÇÃO 2: Umidade moderada
    digitalWrite(rele_pin, LOW);  // Desliga o relé (DESLIGA a bomba)
    lcd.setCursor(0, 1);
    lcd.print("UMIDADE MODERADA");
  }
  else {
    // CONDIÇÃO 3: Solo está úmido ("Umidade Boa")
    digitalWrite(rele_pin, LOW);  // Desliga o relé (DESLIGA a bomba)
    lcd.setCursor(0, 1);
    lcd.print("UMIDADE BOA");
  }

  // Intervalo de 1 segundo entre as leituras para estabilidade
  delay(1000);
}
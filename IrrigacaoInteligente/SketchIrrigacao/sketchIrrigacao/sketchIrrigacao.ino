/*
  Projeto: Irrigador Automático com Display LCD I2C
  Autor: Alex Junior Possidonio
  Data: 05/11/2025 

  Descrição:
  Este código implementa um sistema automatizado de irrigação. Ele utiliza um sensor
  de umidade do solo (higrômetro) para monitorar as condições da terra em tempo real.
  O valor lido é convertido para porcentagem e exibido em um display LCD I2C 16x2.

  Um módulo relé é acionado para ligar uma bomba d'água sempre que a umidade
  cair abaixo de um nível pré-determinado (48%), desligando-a quando o solo
  atinge um nível ideal.

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

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==== DEFINIÇÃO DE PINOS ====
// Sensor de umidade do solo
#define pino_sensor A0

// Relé (bomba d’água)
#define rele_pin 2

// ==== CONFIGURAÇÃO DO LCD I2C ====
// Endereço do módulo I2C (geralmente 0x27 ou 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// ==== VARIÁVEIS ====
int valor_analogico = 0;
int umidade = 0;

// ==== CONFIGURAÇÃO INICIAL ====
void setup() {
  Serial.begin(9600);

  pinMode(pino_sensor, INPUT);
  pinMode(rele_pin, OUTPUT);

  // Garante que o relé comece desligado
  digitalWrite(rele_pin, LOW);

  // Inicializa o LCD I2C
  lcd.init();      
  lcd.backlight(); // Liga a luz de fundo do LCD

  lcd.setCursor(0, 0);
  lcd.print("SISTEMA ATIVO");
  delay(2000);
  lcd.clear();
}

// ==== LOOP PRINCIPAL ====
void loop() {
  // Lê o sensor de umidade
  valor_analogico = analogRead(pino_sensor);

  // Converte para porcentagem (0% = seco, 100% = úmido)
  umidade = map(valor_analogico, 1023, 0, 0, 100);

  // Mostra no monitor serial
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println("%");

  // Mostra no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.print("%");

  // Controle do relé
  if (umidade < 48) {
    // Solo seco → liga bomba
    digitalWrite(rele_pin, HIGH);  // Liga bomba
    lcd.setCursor(0, 1);
    lcd.print("SOLO SECO-REGAR");
  } 
  else if (umidade >= 48 && umidade <= 64) {
    // Umidade média → bomba desligada
    digitalWrite(rele_pin, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("UMIDADE MODERADA");
  } 
  else {
    // Solo úmido → bomba desligada
    digitalWrite(rele_pin, LOW);
    lcd.setCursor(0, 1);
    lcd.print("UMIDADE BOA");
  }

  delay(1000);  // intervalo entre leituras
}
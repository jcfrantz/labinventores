# Irrigador Automático com Display LCD I2C

Este projeto implementa um sistema automatizado de irrigação utilizando um Arduino Uno. Ele monitora a umidade do solo em tempo real com um sensor higrômetro e exibe o valor em porcentagem em um display LCD 16x2.

Um módulo relé controla uma bomba d'água, acionando-a automaticamente quando o solo fica seco (abaixo de um limite pré-definido) e desligando-a quando atinge um nível de umidade adequado. Este projeto é ideal para automação residencial, como vasos de plantas ou pequenas hortas caseiras.

## Funcionalidades

- **Monitoramento de Umidade**: Utiliza um sensor de umidade do solo (higrômetro) para leituras em tempo real.
- **Feedback Visual**: Exibe a porcentagem de umidade e o status do sistema ("SOLO SECO", "UMIDADE MODERADA", "UMIDADE BOA") em um display LCD I2C 16x2.
- **Controle Automatizado da Bomba**: Aciona um módulo relé de 1 canal para ligar ou desligar uma bomba d'água com base nos níveis de umidade.
- **Monitoramento Serial**: Envia os valores de umidade para o Monitor Serial para depuração e calibração.

## Componentes Utilizados

- **Arduino Uno** (ou equivalente)
- **Sensor de Umidade do Solo (Higrômetro)**
- **Módulo Relé de 1 Canal (5V)**
- **Display LCD I2C 16x2**
- **Bomba d'água pequena** (compatível com a especificação do relé)
- **Jumpers e Protoboard**

## Esquema de Conexão

O esquema de conexão está detalhado nos comentários do próprio script.

**Sensor de Umidade do Solo:**
| Sensor | Pino no Arduino |
| :--- | :--- |
| VCC | 5V |
| GND | GND |
| A0 | A0 |

**Módulo Relé:**
| Módulo | Pino no Arduino |
| :--- | :--- |
| VCC | 5V |
| GND | GND |
| IN | 2 |

**Display LCD I2C:**
| LCD | Pino no Arduino |
| :--- | :--- |
| VCC | 5V |
| GND | GND |
| SDA | A4 (SDA) |
| SCL | A5 (SCL) |

## Como Funciona

1.  **Leitura do Sensor**:
    - O pino analógico `A0` lê o valor do sensor de umidade. No Arduino, valores mais altos (próx. 1023) indicam solo seco, e valores mais baixos (próx. 0) indicam solo muito úmido.
2.  **Conversão para Porcentagem**:
    - O valor analógico lido é invertido e mapeado para uma escala de porcentagem mais intuitiva (0% = seco, 100% = úmido) usando a função `map(valor_analogico, 1023, 0, 0, 100)`.
3.  **Exibição no LCD**:
    - A porcentagem de umidade atual é exibida na primeira linha do LCD.
4.  **Controle da Irrigação**:
    - O código verifica a porcentagem de umidade e atualiza a segunda linha do LCD e o estado do relé (Pino 2):
    - **Abaixo de 48%**: O sistema considera o solo seco. O pino `2` é definido como `HIGH`, acionando o relé (ligando a bomba). O LCD exibe "SOLO SECO-REGAR".
    - **Entre 48% e 64%**: O sistema considera a umidade moderada. O pino `2` é mantido em `HIGH` (mantendo a bomba ligada). O LCD exibe "UMIDADE MODERADA".
    - **Acima de 64%**: O sistema considera o solo com boa umidade. O pino `2` é definido como `LOW`, desligando o relé e a bomba. O LCD exibe "UMIDADE BOA".

## Código Arduino

Aqui está o código-fonte completo do projeto:

```cpp
/*
  Projeto: Irrigador Automático com Display LCD I2C
  Autor: Alex Junior Possedonio
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
| Sensor Umidade | Arduino |       | Módulo Relé   | Arduino   |
  ---------------------------------   -----------------------------------
|     VCC        |   5V    |       |     VCC       |    5V     |
|     GND        |   GND   |       |     GND       |    GND    |
|     A0         |   A0    |       |     IN        |    2      |
  ---------------------------------   -----------------------------------
|   LCD I2C      | Arduino |
  ---------------------------------
|     VCC        |   5V    |
|     GND        |   GND   |
|     SDA        |   A4    | (SDA)
|     SCL        |   A5    | (SCL)
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

#include <SPI.h>
#include <LoRa.h>
 
//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
 
#define trigPin 12
#define echoPin 13
int distance;
 
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6
 
//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
 
//packet counter
int counter = 0;
String LoRaData;
String outgoing;              // outgoing message
int relay1Status;
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 50;     
String Mymessage;
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
 
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
 
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  Serial.println("LoRa Sender Test");
 
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Inicializacao LoRa OK!");
  display.setCursor(0,10);
  display.print("Inicializacao LoRa OK!");
  display.display();
  delay(2000);
}
 
void loop() {
   if (millis() - lastSendTime > interval) {
    // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  
  delayMicroseconds(5);
 
 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
int  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance:
//  distance = duration*0.034/2;
  
distance = (duration/2) / 29.1;
  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
int waterLevelPer = map(distance,22,51, 100, 0);
if(waterLevelPer<0)
{
  waterLevelPer=0;
  }
if (waterLevelPer>100)
{
  waterLevelPer=100;
  }
  Serial.print("Distancia = ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println(waterLevelPer);
    Mymessage = Mymessage + distance +","+ waterLevelPer;  
    sendMessage(Mymessage);
     display.clearDisplay();
  display.setCursor(0,0);
  display.println("Transmissor LORA");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("Pacote LoRa enviado");
  display.setCursor(0,30);
  display.print("UNIFEBE");      
  display.display();
  delay(1000);
    
    delay(100);
    Mymessage = "";
  
 //Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(50) + 100;  
    
    }
     
}
 
 
 
 
void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}
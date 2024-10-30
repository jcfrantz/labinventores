#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
 
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
#define led 12
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
 
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
byte msgCount = 0;            // count of outgoing messages
 
String outgoing;              // outgoing message
String LoRaData;
byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 50;          // interval between sends
String statusmessage = "";

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  pinMode(led,OUTPUT);
 
 
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
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("LORA RECEIVER ");
  display.display();
 
  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
 
  if (!LoRa.begin(BAND)) {
    Serial.println("Inic. LoRa Falha!");
    while (1);
  }
  Serial.println("Inici. LoRa OK!");
  display.setCursor(0,10);
  display.println("Inic. LoRa OK!");
  display.display();  

}
 
void loop() {
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
 
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      //Serial.print(LoRaData);
    }
 
String q = getValue(LoRaData, ',', 0); 
String r = getValue(LoRaData, ',', 1); 
int distance = q.toInt();
int waterLevelPer = r.toInt();
Serial.print("distance = ");
Serial.println(distance);
Serial.print("waterLevelPer = ");
Serial.println(waterLevelPer);
 
 
    String rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
    //Serial.println(rssi);
 
  
  delay(10);
 
  
   // Dsiplay information
   display.clearDisplay();
   display.setCursor(5,0);
    display.setTextSize(2);
   display.print("N-A-Dist.");
 
   display.setCursor(0,25);
    display.setTextSize(2);
//   display.print("Dist:");
//   display.setCursor(70,15);
//    display.setTextSize(2);
//   display.print(distance);
       display.print("Dist:"+String(distance)+"cm");    
 
   display.setCursor(0,50);
    display.setTextSize(2);
   //display.print("W-L:");
    display.print("UNIFEBE");    
 
//   display.setCursor(92,40);
//    display.setTextSize(2);
//   display.print(waterLevelPer);
   display.display();   
  
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
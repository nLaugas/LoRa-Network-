#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHTPIN 13

#define DHTTYPE    DHT11 
DHT dht(DHTPIN,DHTTYPE);



#include <SPI.h>
#include <LoRa.h>       // https://github.com/sandeepmistry/arduino-LoRa


//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas

// #include <U8x8lib.h>

#define OFF 0   // For LED
#define ON 1

// SPI LoRa Radio
#define LORA_SCK 5        // GPIO5 - SX1276 SCK
#define LORA_MISO 19     // GPIO19 - SX1276 MISO
#define LORA_MOSI 27    // GPIO27 -  SX1276 MOSI
#define LORA_CS 18     // GPIO18 -   SX1276 CS
#define LORA_RST 14   // GPIO14 -    SX1276 RST
#define LORA_IRQ 26  // GPIO26 -     SX1276 IRQ (interrupt request)

// I2C OLED Display works with SSD1306 driver
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16


#define temperature 23
unsigned long previousMillis = 0;
/* Pick One. Hardware I2C does NOT work! This article helped: https://robotzero.one/heltec-wifi-kit-32/ 
* TTGo boards similar to Heltec boards, LED_BUILTIN = 2 instead of pin 25
* Some OLED displays don't handle ACK correctly so SW I2C works better. Thank you Olikraus!
* TTGo OLED has pin 16 reset unlike other OLED displays
*/


const int blueLED = LED_BUILTIN; 
int counter = 0;

void setup() {



//  Serial.begin(115200);
  //while (!Serial);

  //Serial.println("LoRa Sender");

  
  // Very important for SPI pin configuration!
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS); 
  
  // Very important for LoRa Radio pin configuration! 
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);         

  pinMode(blueLED, OUTPUT); // For LED feedback

  if (!LoRa.begin(915E6)) {
    //Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // The larger the spreading factor the greater the range but slower data rate
  // Send and receive radios need to be set the same
  LoRa.setSpreadingFactor(12); // ranges from 6-12, default 7 see API docs

  // Change the transmit power of the radio
  // Default is LoRa.setTxPower(17, PA_OUTPUT_PA_BOOST_PIN);
  // Most modules have the PA output pin connected to PA_BOOST, gain 2-17
  // TTGO and some modules are connected to RFO_HF, gain 0-14
  // If your receiver RSSI is very weak and little affected by a better antenna, change this!
  LoRa.setTxPower(14, PA_OUTPUT_RFO_PIN);
  
}



void loop() {

  
      //Serial.print("Sending packet: ");
      //Serial.println(counter);
      //float t = dht.readTemperature();
          // Read temperature as Fahrenheit (isFahrenheit = true)
          //float t = dht.readTemperature(true);
          // Check if any reads failed and exit early (to try again).
          
          
      //float h = dht.readHumidity();
        
      digitalWrite(blueLED, ON);  // Turn blue LED on
      // send packet
      LoRa.beginPacket();
      //LoRa.print(t);
      LoRa.print(temperature+random(1,5));
      LoRa.endPacket();
      digitalWrite(blueLED, OFF); // Turn blue LED off
      delay(10000);
    
      
}

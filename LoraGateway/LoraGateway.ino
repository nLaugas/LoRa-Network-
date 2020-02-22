/*
  WriteSingleField
  
  Description: Writes a value to a channel on ThingSpeak every 20 seconds.
  
  Hardware: ESP32 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires installation of EPS32 core. See https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md for details. 
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2018, The MathWorks, Inc.
*/


#include <SPI.h>
#include <LoRa.h>
#include "ThingSpeak.h"
#include "secrets.h"
#include <WiFi.h>



// SPI LoRa Radio
#define LORA_SCK 5        // GPIO5 - SX1276 SCK
#define LORA_MISO 19     // GPIO19 - SX1276 MISO
#define LORA_MOSI 27    // GPIO27 - SX1276 MOSI
#define LORA_CS 18     // GPIO18 - SX1276 CS
#define LORA_RST 14   // GPIO14 - SX1276 RST
#define LORA_IRQ 26  // GPIO26 - SX1276 IRQ (interrupt request)

String rssi = "";
String packet = "";



//char ssid[] = "Fibertel WiFi499 2.4GHz";   // your network SSID (name) 
char ssid[] = "campus";
char pass[]  = "";
//char pass[] = "0043630189";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 995721;
const char * myWriteAPIKey = "BLOQEJXHSD11TJ00";

int number = 0;




void setup() {
  Serial.begin(115200);  //Initialize serial
  
   Serial.println("LoRa Receiver");
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak





  // Very important for SPI pin configuration!
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS); 
  
  // Very important for LoRa Radio pin configuration! 
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);         

  
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // The larger the spreading factor the greater the range but slower data rate
  // Send and receive radios need to be set the same
  LoRa.setSpreadingFactor(12);  // ranges from 6-12, default 7 see API docs

}

void loop() {



int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    
    Serial.print("Received packet '");


    // read packet
    packet = "";                   // Clear packet
    while (LoRa.available()) {
      packet += (char)LoRa.read(); // Assemble new packet
    }
    rssi = LoRa.packetRssi();
   
    Serial.println(packet + "' with RSSI " + rssi); 
     // Connect or reconnect to WiFi
            
          
  if(WiFi.status() != WL_CONNECTED){
              Serial.print("Attempting to connect to SSID: ");
              Serial.println(ssid);
              while(WiFi.status() != WL_CONNECTED){
                WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
                Serial.print(".");
                delay(5000);     
              } 
              Serial.println("\nConnected.");
            }
            // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
            // pieces of information in a channel.  Here, we write to field 1.
            int x = ThingSpeak.writeField(myChannelNumber, 1, packet, myWriteAPIKey);
            if(x == 200){
              Serial.println("Channel update successful.");
            }
            else{
              Serial.println("Problem updating channel. HTTP error code " + String(x));
            }
            
            // change the value
            
  }
           
            
           
}

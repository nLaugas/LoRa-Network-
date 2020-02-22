#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  //Serial.begin(9600);
  //while (!Serial);

  //Serial.println("LoRa Sender");

  if (!LoRa.begin(928E6)) {
    //Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(20);
  
}

void loop() {
  //Serial.print("Sending packet: ");
  //Serial.println(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);

}

 

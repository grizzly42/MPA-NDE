#include <SPI.h>
#include <LoRa.h>

int counter = 0;

#define BW 125E3 
#define TXPWR 10 // in dBm
#define SF 12 //spreading factor

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSignalBandwidth(BW);
  LoRa.setTxPower(TXPWR);
  LoRa.setSpreadingFactor(SF);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}

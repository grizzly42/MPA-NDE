#include <SPI.h>
#include <RH_RF95.h>

#define STAT_LED 3

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 868.0
#define RF95_PWR 23 // Zvýšeno na max 23 dBm
String OPERATOR = "DKT";

RH_RF95 rf95(RFM95_CS, RFM95_INT);

unsigned long lastSendTime = 0;
int packetCount = 0;

void resetLoRa() {
  digitalWrite(RFM95_RST, LOW);
  delay(100);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
}

void setup() {
  pinMode(STAT_LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  delay(100);

  Serial.println("Booting LoRa Radio");

  resetLoRa();

  digitalWrite(STAT_LED, HIGH);
  while (!rf95.init()) {
    Serial.println("LoRa Radio Init Failed");
    resetLoRa();
    delay(500);
  }
  Serial.println("LoRa Radio OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Freq set to: "); Serial.print(RF95_FREQ); Serial.println("MHz");

  rf95.setTxPower(RF95_PWR, false);
  Serial.print("TXPower set to: "); Serial.print(RF95_PWR); Serial.println("dBm");

  Serial.println("Ready!");
}

void loop() {
  if (millis() - lastSendTime >= 3000) { // Každou sekundu pošle paket
    lastSendTime = millis();
    
    String message = OPERATOR + ": Packet " + String(packetCount++);
    int messageSize = message.length() + 1;
    char radiopacket[messageSize];
    message.toCharArray(radiopacket, messageSize);

    Serial.println("Před odesláním...");

    rf95.send((uint8_t *)radiopacket, messageSize);
    rf95.waitPacketSent(); // Ujisti se, že je paket odeslán

    Serial.println("Po odeslání!");

    resetLoRa(); // Reset po každém paketu pro jistotu
  }
}

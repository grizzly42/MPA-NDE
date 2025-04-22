#include <SPI.h>
#include <LoRa.h>

#include "pakety.h"

#define LED PD3        // LED pro výstup
#define BTN PD4        // Tlačítko pro aktivaci

#define SWITCH1_PIN 16  // Koncový spínač 1
#define OUTPUT1_PIN 17  // Výstup pro spínač 1

#define SWITCH2_PIN 6  // Koncový spínač 2
#define OUTPUT2_PIN 8  // Výstup pro spínač 2

// safety countdown, test setting 1s
#define CNTDWN 1000

#define TIMEOUT 5000  // Timeout v ms

#define BW 125E3
#define TXPWR 10 // in dBm
#define SF 12 //spreading factor

enum State {
  PROG,
  COUNTDOWN,
  DEPLOYING,
  TRANSMITING
};

State currentState = PROG;

const float referenceVoltage = 3.3;   // nebo 5.0, podle toho, co přivádíš na AREF
const float voltageDividerRatio = 72.0 / (24.0 + 72.0);

int counter = 0;
int p_counter = 0;

void setup() {
  Serial.begin(9600);

  pinMode(SWITCH1_PIN, INPUT_PULLUP);
  pinMode(SWITCH2_PIN, INPUT_PULLUP);

  pinMode(OUTPUT1_PIN, OUTPUT);
  pinMode(OUTPUT2_PIN, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);

  digitalWrite(LED, LOW);
  digitalWrite(OUTPUT1_PIN, LOW);
  digitalWrite(OUTPUT2_PIN, LOW);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  } else {
    Serial.println("Starting LoRa succeed");
  }

  LoRa.setSignalBandwidth(BW);
  LoRa.setTxPower(TXPWR);
  LoRa.setSpreadingFactor(SF);

  // Signalizace zapnutí
  digitalWrite(LED, HIGH);
  delay(300);
  digitalWrite(LED, LOW);
  Serial.println("setup ok");

}

// START_COUNTDOWN,
//  DEPLOYING,
//  TRANSMITING

void loop() {
  delay(30);
  switch (currentState) {
    case PROG:

      if (digitalRead(BTN) == LOW) {
        Serial.println("prog_mode");
        while (1);
      } else {
        currentState = COUNTDOWN;
      }

      break;
    case COUNTDOWN:

      Serial.println("satet: COUNTDOWN");
      delay(CNTDWN);
      currentState = DEPLOYING;

      break;

    case DEPLOYING:

      Serial.println("satet: DEPLOYING");

      // Zapnutí přepalovacích odporů
      digitalWrite(OUTPUT1_PIN, HIGH);
      digitalWrite(OUTPUT2_PIN, HIGH);

      if (digitalRead(SWITCH1_PIN) == HIGH) {
        digitalWrite(OUTPUT1_PIN, LOW);
      }
      if (digitalRead(SWITCH2_PIN) == HIGH) {
        digitalWrite(OUTPUT2_PIN, LOW);
      }

      if (digitalRead(SWITCH1_PIN) == HIGH && digitalRead(SWITCH2_PIN) == HIGH) {
        currentState = TRANSMITING;
      }

      break;
    case TRANSMITING:
      delay(5000);

      int raw = analogRead(PC0); // nebo A0
      float voltageADC = (raw / 1023.0) * referenceVoltage;
      float batteryVoltage = voltageADC / voltageDividerRatio;

      Serial.print("Sending packet: ");
      Serial.println(counter);

      // send packet
      LoRa.beginPacket();

      LoRa.print("pct n. ");
      LoRa.println(counter);

      LoRa.print("VBatt: ");
      LoRa.println(batteryVoltage);

      LoRa.println(pakety[p_counter]);

      LoRa.endPacket();

      if (p_counter >= packet_count - 1) {
        p_counter = 0;
      } else {
        p_counter++;
      }
      counter++;
      break;
  }



}

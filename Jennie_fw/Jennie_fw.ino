#include <SPI.h>
#include <LoRa.h>

#include "pakety.h"

#define LED PD3        // LED pro výstup
#define BTN PD4        // Tlačítko pro aktivaci

#define SWITCH1_PIN 16  // Koncový spínač 1
#define OUTPUT1_PIN 17  // Výstup pro spínač 1

#define SWITCH2_PIN 6  // Koncový spínač 2
#define OUTPUT2_PIN 8  // Výstup pro spínač 2

#define PROG_EN PC4
#define JUST_GND PC5

#define TIMEOUT 5000  // Timeout v ms

#define BW 125E3
#define TXPWR 10 // in dBm
#define SF 12 //spreading factor

bool start_depl = false;
unsigned long timeout_state = 0;
bool output1_active = false;
bool output2_active = false;

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

  pinMode(PROG_EN, INPUT_PULLUP);
  pinMode(JUST_GND, OUTPUT);
  digitalWrite(JUST_GND, LOW);

  digitalWrite(LED, LOW);
  digitalWrite(OUTPUT1_PIN, LOW);
  digitalWrite(OUTPUT2_PIN, LOW);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  } else {
    Serial.println("Starting LoRa succeed");
  }

  // Bezpečnostní pojistka, pokud nechceme deployment
  while (digitalRead(PROG_EN) == LOW) {
    Serial.println("Programming wire connected");
    delay(1000);
  }


  LoRa.setSignalBandwidth(BW);
  LoRa.setTxPower(TXPWR);
  LoRa.setSpreadingFactor(SF);

  // Signalizace zapnutí
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("setup ok");

}

void loop() {
  delay(30);
  Serial.println("loop start");
  if (digitalRead(BTN) == LOW || !start_depl) {
    start_depl = true;
    Serial.println("Tlačítko stisknuto, start!");

    // Aktivace výstupů
    digitalWrite(LED, HIGH);
    digitalWrite(OUTPUT1_PIN, HIGH);
    digitalWrite(OUTPUT2_PIN, HIGH);

    output1_active = true;
    output2_active = true;
  }

  if (start_depl) {
    Serial.println("Depl start");
    // Kontrola výstupu 1
    if (output1_active) {
      if (digitalRead(SWITCH1_PIN) == HIGH) {
        digitalWrite(OUTPUT1_PIN, LOW);
        output1_active = false;
        Serial.println("OUTPUT 1 OFF");
      }
    }

    // Kontrola výstupu 2
    if (output2_active) {
      if (digitalRead(SWITCH2_PIN) == HIGH) {
        digitalWrite(OUTPUT2_PIN, LOW);
        output2_active = false;
        Serial.println("OUTPUT 2 OFF");
      }
    }
    if (!output1_active && !output2_active) {
      //start_depl = false;

      delay(5000);

      int raw = analogRead(PC0); // nebo A0
      float voltageADC = (raw / 1023.0) * referenceVoltage;
      float batteryVoltage = voltageADC / voltageDividerRatio;

      digitalWrite(LED, LOW);
      Serial.println("Sekvence dokončena.");

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
    }
  }
}

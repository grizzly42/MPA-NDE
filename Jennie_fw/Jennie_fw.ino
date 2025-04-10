#include <SPI.h>
#include <LoRa.h>

#include "pakety.h" //file with messages to be sent

#define LED PD3        // LED indication
#define BTN PD4        // Activation button (can be used to deploy antenna)

#define SWITCH1_PIN 16  // Deployment switch feedback 1
#define OUTPUT1_PIN 17  // Transistor to switch current to depolyment resistor 1 

#define SWITCH2_PIN 6  // Deployment switch feedback 2
#define OUTPUT2_PIN 8  // Transistor to switch current to depolyment resistor 1

#define ENABLE_PROG PC4

#define TIMEOUT 5000  // Timeout in ms

#define BW 125E3 // Bandwidth of LoRa modulation
#define TXPWR 10 // in dBm
#define SF 12 //spreading factor

#define REF_VOLT = 3.3;
#define DIVIDER_RATIO = 72.0 / (24.0 + 72.0);

bool start_depl = false; // Enable deployment sequence
bool output1_active = false; // Activatation of deployment trasistors
bool output2_active = false;

long max_milliseconds = 36000; //transmitt policy (max 36 s per hour)
int send_start = 0;

int counter = 0; // Message counter
int p_counter = 0; // Array pointer

bool legal_trx = false;

void setup() {

  Serial.begin(9600);
   // Setting the pin modes
  pinMode(SWITCH1_PIN, INPUT_PULLUP);
  pinMode(SWITCH2_PIN, INPUT_PULLUP);
  pinMode(ENABLE_PROG, INPUT_PULLDOWN);
  pinMode(OUTPUT1_PIN, OUTPUT);
  pinMode(OUTPUT2_PIN, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);

  digitalWrite(LED, LOW);
  digitalWrite(OUTPUT1_PIN, LOW);
  digitalWrite(OUTPUT2_PIN, LOW);

  while(digitalRead(ENABLE_PROG) == true){} // Wait to be programmed

  // Set the trx freq. at 868MHz
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSignalBandwidth(BW); // Setting lora modulation
  LoRa.setTxPower(TXPWR);
  LoRa.setSpreadingFactor(SF);

  // Indication of on state
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("setup ok");

}

void loop() {
  delay(30);
  Serial.println("loop start");
  if (digitalRead(BTN) == LOW || !start_depl) { // Start of deployment sequence (start when RBF is removed or BTN is pressed)
    start_depl = true;
    Serial.println("Tlačítko stisknuto, start!");

    // Output activation - heating up the deployment resistors
    digitalWrite(LED, HIGH);
    digitalWrite(OUTPUT1_PIN, HIGH);
    digitalWrite(OUTPUT2_PIN, HIGH);

    output1_active = true;
    output2_active = true;
  }

  if (start_depl) {
    
    if (output1_active) {
      if (digitalRead(SWITCH1_PIN) == HIGH) {
        digitalWrite(OUTPUT1_PIN, LOW);
        output1_active = false;
        Serial.println("OUTPUT 1 OFF");
      }
    }

    if (output2_active) {
      if (digitalRead(SWITCH2_PIN) == HIGH) {
        digitalWrite(OUTPUT2_PIN, LOW);
        output2_active = false;
        Serial.println("OUTPUT 2 OFF");
      }
    }
    if (!output1_active && !output2_active && (!legal_trx || max_milliseconds > 0)) {
      
      Serial.println("antenna deployment successful");
      delay(5000);

      int raw = analogRead(PC0); // nebo A0
      float voltageADC = (raw / 1023.0) * REF_VOLT;
      float batteryVoltage = voltageADC / DIVIDER_RATIO;

      digitalWrite(LED, LOW);
      
      // send packet
      LoRa.beginPacket();

      LoRa.print("pct n. ");
      LoRa.println(counter);

      LoRa.print("VBatt: ");
      LoRa.println(batteryVoltage);

      LoRa.println(pakety[p_counter]);

      send_start = millis();
      LoRa.endPacket();
      max_milliseconds = max_milliseconds - (millis()-send_start);

      Serial.println("packet sent");
      
      if (p_counter >= packet_count - 1) {
        p_counter = 0;
      } else {
        p_counter++;
      }
      counter++;
    }
  }
}

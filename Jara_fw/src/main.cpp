#include <LoRaNow.h>

#define OUTPUT_PIN 7
#define STOP_PIN 6

bool outputActive = false;

// Deklarace callbacku
void onLoRaMessage();

void setup() {
    Serial.begin(115200);
    pinMode(OUTPUT_PIN, OUTPUT);
    pinMode(STOP_PIN, INPUT_PULLUP);

    // Inicializace LoRaNow
    LoRaNow.begin();
    LoRaNow.setFrequency(868E6);
    LoRaNow.onMessage(onLoRaMessage);
    
    Serial.println("LoRaNow Ready");
}

void loop() {
    LoRaNow.loop(); // Zpracování LoRa zpráv

    // Kontrola příkazu přes UART
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command == "START") {
            activateOutput();
        }
    }

    // Kontrola ukončení
    if (outputActive && digitalRead(STOP_PIN) == LOW) {
        deactivateOutput();
    }
}

// Callback pro přijatou zprávu z LoRa
void onLoRaMessage() {
    if (!LoRaNow.available()) return;
    uint8_t payload[LoRaNow.available()];
    LoRaNow.read(payload, sizeof(payload));
    
    String receivedCommand = String((char*)payload);
    receivedCommand.trim();

    Serial.print("LoRa Received: ");
    Serial.println(receivedCommand);

    if (receivedCommand == "START") {
        activateOutput();
    }
}

void activateOutput() {
    outputActive = true;
    digitalWrite(OUTPUT_PIN, HIGH);
    Serial.println("OUTPUT ON");
}

void deactivateOutput() {
    outputActive = false;
    digitalWrite(OUTPUT_PIN, LOW);
    Serial.println("OUTPUT OFF");
}

#define LED_PIN PD3  // Pin pro LED
#define BTN_PIN 6   // Pin pro tlačítko

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);  // Tlačítko s interním pull-up rezistorem
}

void loop() {
  // Pokud je tlačítko stisknuté (stav LOW), rozsvítí LED
  if (digitalRead(BTN_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

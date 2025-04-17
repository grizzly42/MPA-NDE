#define LED PD3

#define SWITCH1_PIN 16  // GPIO16 = physical pin 27 (PC2)
#define SWITCH2_PIN 6   // GPIO6  = physical pin 12 (PD6)

void setup() {
  // Inicializace LED jako výstup
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Koncové spínače jako vstup s vnitřním pullupem
  pinMode(SWITCH1_PIN, INPUT_PULLUP);
  pinMode(SWITCH2_PIN, INPUT_PULLUP);
}

void loop() {
  // Spínače používají pullup, takže sepnutí znamená LOW
  bool switch1_pressed = digitalRead(SWITCH1_PIN) == LOW;
  bool switch2_pressed = digitalRead(SWITCH2_PIN) == LOW;

  if (switch1_pressed || switch2_pressed) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  delay(10); // malé zpoždění pro odrušení chvění kontaktu
}

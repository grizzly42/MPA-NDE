const float referenceVoltage = 1.1;   // používáme vnitřní referenci 1.1V
const float voltageDividerRatio = 24.0 / (24.0 + 72.0); // převrácený poměr děliče

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL); // nastavíme referenci na 1.1V
}

void loop() {
  int raw = analogRead(PC0); // čtení z PC0 = A0
  
  
  float voltageADC = (raw / 1023.0) * referenceVoltage; // napětí na vstupu A0
  Serial.print("VADC: ");
  Serial.print(voltageADC);
  Serial.println(" V");
  
  float batteryVoltage = voltageADC / voltageDividerRatio; // výpočet napětí baterie

  Serial.print("Napeti baterie: ");
  Serial.print(batteryVoltage);
  Serial.println(" V");

  delay(1000);
}

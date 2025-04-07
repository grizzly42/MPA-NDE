const float referenceVoltage = 3.3;   // nebo 5.0, podle toho, co přivádíš na AREF
const float voltageDividerRatio = 72.0 / (24.0 + 72.0); 

void setup() {
  Serial.begin(9600);
  //analogReference(EXTERNAL); // používáme externí referenci na pinu AREF
}

void loop() {
  int raw = analogRead(PC0); // nebo A0

  float voltageADC = (raw / 1023.0) * referenceVoltage;
  float batteryVoltage = voltageADC / voltageDividerRatio;

  Serial.print("VADC: ");
  Serial.print(voltageADC, 2);
  Serial.print(" V | Napeti baterie: ");
  Serial.print(batteryVoltage, 2);
  Serial.println(" V");

  delay(1000);
}

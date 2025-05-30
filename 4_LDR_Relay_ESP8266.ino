#define LDR_PIN A0
#define RELAY_PIN D1

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  bool terang = ldrValue > 500;

  if (terang) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(1000);
}
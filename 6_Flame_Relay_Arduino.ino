#define FLAME_PIN A0
#define RELAY_PIN 7

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  int flameValue = analogRead(FLAME_PIN);

  if (flameValue < 300) {
    digitalWrite(RELAY_PIN, HIGH);  // Trigger alarm or cut power
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(1000);
}
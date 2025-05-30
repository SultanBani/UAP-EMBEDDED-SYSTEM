#define SOIL_PIN A0
#define RELAY_PIN 7

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  int soilValue = analogRead(SOIL_PIN);

  if (soilValue < 400) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(1000);
}
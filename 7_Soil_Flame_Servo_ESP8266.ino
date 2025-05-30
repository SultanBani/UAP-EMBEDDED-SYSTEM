#include <Servo.h>

#define SOIL_PIN A0
#define FLAME_PIN A0
#define SERVO_PIN D2

Servo servo;

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
}

void loop() {
  int soilValue = analogRead(SOIL_PIN);
  int flameValue = analogRead(FLAME_PIN);

  if (flameValue < 300) {
    servo.write(90);  // Close hatch or protective measure
  } else if (soilValue < 400) {
    servo.write(0);   // Open for watering
  }

  delay(1000);
}
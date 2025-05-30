#include <Servo.h>

#define ANALOG_PIN A0
#define SERVO_PIN D2

Servo servo;

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
}

void loop() {
  int nilai = analogRead(ANALOG_PIN);
  bool hujan = nilai < 400;

  if (hujan) {
    servo.write(90);
  } else {
    servo.write(0);
  }

  delay(1000);
}
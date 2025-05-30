//Master judul 2

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String message = Serial.readString();
    Wire.beginTransmission(8);
    Wire.write(message.c_str());
    Wire.endTransmission();
    Serial.println("Message sent: " + message);
  }
  delay(1000);
}

// slave judul 2

#include <Wire.h>

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  delay(1000);
}

void receiveEvent(int bytes) {
  String message = "";
  while (Wire.available()) {
    char c = Wire.read();
    message += c;
  }
  Serial.println("Message received: " + message);
}

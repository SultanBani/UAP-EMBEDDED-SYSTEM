
// Smartfarm system using Soil, Raindrops, Ultrasonic, controlling Pump
#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME "SmartFarm"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

#define SOIL_PIN A0
#define RAIN_PIN D1
#define TRIG_PIN D2
#define ECHO_PIN D3
#define PUMP_PIN D5

BlynkTimer timer;

void sendSensor() {
  int soil = analogRead(SOIL_PIN);
  int rain = digitalRead(RAIN_PIN);
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Blynk.virtualWrite(V3, soil);
  Blynk.virtualWrite(V4, rain);
  Blynk.virtualWrite(V5, distance);

  if (soil < 500 && rain == HIGH) {
    digitalWrite(PUMP_PIN, HIGH);
  } else {
    digitalWrite(PUMP_PIN, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(RAIN_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  timer.setInterval(3000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

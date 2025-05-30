
// Fire prevention system using Flame sensor and DHT, controlling Relay
#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME "FirePrevention"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

#define FLAME_PIN D1
#define DHTPIN D2
#define DHTTYPE DHT11
#define RELAY_PIN D5

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor() {
  int flame = digitalRead(FLAME_PIN);
  float t = dht.readTemperature();

  Blynk.virtualWrite(V6, flame);
  Blynk.virtualWrite(V7, t);

  if (flame == LOW || t > 40.0) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(FLAME_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

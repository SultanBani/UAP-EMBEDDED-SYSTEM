#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "SmartFarming"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

// WiFi
char ssid[] = "NAMA_WIFI";
char pass[] = "PASSWORD_WIFI";

// Pin
#define DHTPIN D5
#define DHTTYPE DHT22
#define TRIG_PIN D6
#define ECHO_PIN D7
#define RELAY_PIN D1
#define SERVO_PIN D2
#define ANALOG_PIN A0

// Global
DHT dht(DHTPIN, DHTTYPE);
Servo servo;
bool manualPompa = false;
bool manualServo = false;

BlynkTimer timer;

// === Manual Kontrol Pompa dari Blynk ===
BLYNK_WRITE(V5) {
  manualPompa = param.asInt();
}

// === Manual Kontrol Servo dari Blynk ===
BLYNK_WRITE(V6) {
  manualServo = param.asInt();
}

// === Fungsi Utama ===
void smartFarming() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  // Ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long durasi = pulseIn(ECHO_PIN, HIGH);
  float jarak = durasi * 0.034 / 2;

  // LDR + Raindrop
  int analogValue = analogRead(ANALOG_PIN);
  bool hujan = analogValue < 400;

  // === Kontrol Pompa ===
  bool autoPompa = (kelembaban < 50 || jarak > 10);
  if (manualPompa) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, autoPompa ? HIGH : LOW);
  }

  // === Kontrol Servo ===
  if (manualServo) {
    servo.write(90);
  } else {
    servo.write(hujan ? 90 : 0);
  }

  // === Kirim ke Blynk ===
  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, kelembaban);
  Blynk.virtualWrite(V2, jarak);
  Blynk.virtualWrite(V3, hujan ? "Hujan" : "Cerah");
  Blynk.virtualWrite(V4, digitalRead(RELAY_PIN)); // LED Pompa

  // === Serial Debug ===
  Serial.println("=== Smart Farming ===");
  Serial.print("Suhu: "); Serial.println(suhu);
  Serial.print("Kelembaban: "); Serial.println(kelembaban);
  Serial.print("Jarak: "); Serial.println(jarak);
  Serial.print("Hujan: "); Serial.println(hujan ? "Ya" : "Tidak");
  Serial.print("Pompa: "); Serial.println(digitalRead(RELAY_PIN));
  Serial.println("=====================");
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  servo.attach(SERVO_PIN);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  timer.setInterval(2000L, smartFarming); // setiap 2 detik
}

void loop() {
  Blynk.run();
  timer.run();
}

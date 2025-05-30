#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

// === Konfigurasi WiFi dan Telegram ===
const char* ssid = "POCO F5";
const char* password = "12345689";
#define BOT_TOKEN "8035251984:AAHL64HmiuNIyGBaa7G9fZFXzBPuLPE-9lE"
#define CHAT_ID "1797000356"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// === Sensor DHT11 ===
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// === Sensor Analog ===
#define MQ135_PIN 34
#define LDR_PIN 35

// === Output Indikator ===
#define LED_STATUS 2
#define BUZZER_PIN 13

// === Timer Kirim Pesan ===
unsigned long lastTime = 0;
unsigned long interval = 6000;

// === Kalibrasi MQ135 ===
const float RL = 10.0;             // Resistor beban 10K ohm
const float R0 = 4.5;              // Nilai R0 dari kalibrasi
const float ADC_RESOLUTION = 4095.0;
const float VREF = 3.3;

// === Fungsi konversi MQ135 ke ppm CO2 ===
float getMQ135PPM(int analogValue) {
  float voltage = analogValue * (VREF / ADC_RESOLUTION);

  // Hindari pembagian dengan nol
  if (voltage < 0.01) return -1;

  float RS = (VREF - voltage) * RL / voltage;
  if (RS <= 0) return -1;

  float ratio = RS / R0;
  float ppm = pow(10, ((log10(ratio) - 1.92) / -0.42));

  if (isinf(ppm) || isnan(ppm)) return -1;

  return ppm;
}

// === Fungsi menentukan status kualitas udara ===
String getAirQualityStatus(float ppm) {
  if (ppm < 400) return "🟢 *Baik*";
  else if (ppm < 1000) return "🟡 *Sedang*";
  else return "🔴 *Buruk*";
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_STATUS, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_STATUS, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung");
  client.setInsecure();
}

void ringBuzzer(int durationMillis) {
  unsigned long startMillis = millis();
  while (millis() - startMillis < durationMillis) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}

void loop() {
  if (millis() - lastTime > interval) {
    lastTime = millis();

    digitalWrite(LED_STATUS, LOW);
    delay(100);
    digitalWrite(LED_STATUS, HIGH);

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int mq135Raw = analogRead(MQ135_PIN);
    int ldrValue = analogRead(LDR_PIN);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Gagal baca DHT11!");
      return;
    }

    float mq135PPM = getMQ135PPM(mq135Raw);
    String mq135Status;

    if (mq135PPM < 0) {
      mq135PPM = 0;
      mq135Status = "❓ *Tidak Valid*";
    } else {
      mq135Status = getAirQualityStatus(mq135PPM);
    }

    String message = "📡 *Data Sensor ESP32*\n";
    message += "🌡 Suhu: " + String(temperature) + " °C\n";
    message += "💧 Kelembaban: " + String(humidity) + " %\n";
    message += "🧪 MQ135: " + String(mq135PPM, 2) + " ppm (" + mq135Status + ")\n";
    message += "💡 LDR (Cahaya): " + String(ldrValue) + "\n";

    String warning = "";
    int warningCount = 0;

    if (temperature > 32) {
      warning += "⚠️ Suhu terlalu panas untuk belajar!\n";
      warningCount++;
    } else if (temperature < 20) {
      warning += "⚠️ Suhu terlalu dingin untuk belajar!\n";
      warningCount++;
    }

    if (humidity < 30) {
      warning += "⚠️ Udara terlalu kering!\n";
      warningCount++;
    } else if (humidity > 70) {
      warning += "⚠️ Udara terlalu lembab!\n";
      warningCount++;
    }

    if (mq135PPM > 1000) {
      warning += "⚠️ Kualitas udara buruk! CO₂ tinggi.\n";
      warningCount++;
    }

    if (ldrValue > 3000) {
      warning += "⚠️ Pencahayaan kurang! Ruangan gelap.\n";
      warningCount++;
    }

    Serial.println("Mengirim ke Telegram...");
    bot.sendMessage(CHAT_ID, message, "Markdown");

    if (warningCount > 0) {
      bot.sendMessage(CHAT_ID, "🚨 *PERINGATAN KONDISI TIDAK NYAMAN:*\n" + warning, "Markdown");
    }

    if (warningCount >= 3) {
      ringBuzzer(3000);
    }
  }
}

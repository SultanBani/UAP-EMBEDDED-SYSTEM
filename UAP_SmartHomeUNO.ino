
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// SENSOR DAN LCD 
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

// PIN 
const int pirPin = 2;         // Sensor gerak
const int ledGerak = 13;      // LED indikator gerakan
const int ledSuhuTinggi = 8;  // LED indikator suhu tinggi

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledGerak, OUTPUT);
  pinMode(ledSuhuTinggi, OUTPUT);

  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.backlight();
}

void loop() {
  int motion = digitalRead(pirPin);
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  //  LED gerakan
  if (motion == HIGH) {
    digitalWrite(ledGerak, HIGH);
  } else {
    digitalWrite(ledGerak, LOW);
  }

  // LED suhu tinggi 
  if (suhu > 35) {
    digitalWrite(ledSuhuTinggi, HIGH);
  } else {
    digitalWrite(ledSuhuTinggi, LOW);
  }

  // Tampilkan LCD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(suhu);
  lcd.print((char)223);
  lcd.print("C H:");
  lcd.print(kelembapan);
  lcd.print("% ");

  lcd.setCursor(0, 1);
  if (motion == HIGH) {
    lcd.print("Gerak: TERDETEKSI ");
  } else {
    lcd.print("Gerak: TIDAK     ");
  }

  // Tampilan ke Serial Monitor 
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" C, Kelembapan: ");
  Serial.print(kelembapan);
  Serial.print(" %, Gerakan: ");
  Serial.print(motion == HIGH ? "Terdeteksi" : "Tidak");
  Serial.print(", Suhu Tinggi: ");
  Serial.println(suhu > 35 ? "YA" : "TIDAK");

  delay(1000);
}

#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Pin untuk sensor ultrasonik
#define PIN_TRIG 3
#define PIN_ECHO 2

// Objek LCD dan Servo
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

// Konstanta dan variabel
const byte servoPin = 9;
const byte servoMinAngle = 0;
const byte servoMaxAngle = 180;
int pos = 90; // posisi tengah default

void setup() {
  Serial.begin(115200);

  // Inisialisasi pin sensor
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistem");
  lcd.setCursor(0, 1);
  lcd.print("Gerbang Otomatis");

  // Inisialisasi Servo
  myservo.attach(servoPin);
  myservo.write(pos); // mulai dari posisi tengah
  delay(2000);
}

void loop() {
  // Kirim sinyal trigger
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Baca durasi echo
  int duration = pulseIn(PIN_ECHO, HIGH);
  int distanceCM = duration / 58;

  Serial.print("Distance (CM): ");
  Serial.println(distanceCM);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak: ");
  lcd.print(distanceCM);
  lcd.print(" cm");

  if (distanceCM > 0 && distanceCM < 20) {
    lcd.setCursor(0, 1);
    lcd.print("Selamat Datang!");
    myservo.write(servoMaxAngle); // buka gerbang
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Menunggu...");
    myservo.write(servoMinAngle); // tutup gerbang
  }

  delay(1000); // jeda 1 detik
}

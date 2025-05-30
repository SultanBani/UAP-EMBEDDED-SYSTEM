//master per3

#include <SPI.h>
const uint8_t MOSI_PIN = 11;
const uint8_t MISO_PIN = 12;
const uint8_t SCK_PIN = 13;
const uint8_t SS_PIN = 10;
void setup() {
 Serial.begin(9600);
 while (!Serial);
 pinMode(MOSI_PIN, OUTPUT);
 pinMode(MISO_PIN, INPUT);
 pinMode(SCK_PIN, OUTPUT);
 pinMode(SS_PIN, OUTPUT);
 SPI.begin();
 digitalWrite(SS_PIN, HIGH);
}
void loop() {
 uint8_t outByte = 0xA5;
 Serial.print("Master kirim: 0b");
 Serial.println(outByte, BIN);
 digitalWrite(SS_PIN, LOW);
 SPI.beginTransaction(SPISettings(1000000, MSBFIRST,
SPI_MODE0));
 SPI.transfer(outByte);
 SPI.endTransaction();
 digitalWrite(SS_PIN, HIGH);
 delay(500);
}

//slave per3

#include <SPI.h>
// Pin SPI
const uint8_t MOSI_PIN = 11;
const uint8_t MISO_PIN = 12;
const uint8_t SCK_PIN = 13;
const uint8_t SS_PIN = 10;
volatile uint8_t lastReceived = 0;
volatile uint32_t recvCount = 0;
void setup() {
 Serial.begin(9600);
 delay(10);
 pinMode(MOSI_PIN, INPUT);
 pinMode(MISO_PIN, OUTPUT);
 pinMode(SCK_PIN, INPUT);
 pinMode(SS_PIN, INPUT_PULLUP);
 SPCR = _BV(SPE) | _BV(SPIE); 
   }
ISR(SPI_STC_vect) {
 lastReceived = SPDR;
 recvCount++;
}
void loop() {
 static uint32_t lastCount = 0;
 if (recvCount != lastCount) {
 Serial.print(">> ISR terjadi #");
 Serial.print(recvCount);
 Serial.print(" , data diterima = 0b");
 Serial.println(lastReceived, BIN);
 lastCount = recvCount;
 }
}

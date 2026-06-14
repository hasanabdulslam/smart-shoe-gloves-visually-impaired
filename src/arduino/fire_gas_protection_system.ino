/*
  Fire & Gas Protection System - Public Portfolio Version
  Prepared, cleaned, documented, and organized by Hassan Abdulsalam Mohammed

  Original uploaded material contained an Arduino code snippet in RTF format.
  This version removes private phone numbers and replaces them with placeholders.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial gsmSerial(9, 10); // RX, TX for GSM module

const int exhaustFanPin = 6;
const int extinguisherPin = 5;
const int buzzerPin = 4;
const int mq2SensorPin = A0;
const int flameSensorPin = 2;

const int gasThreshold = 300;
const char emergencyNumber[] = "+XXX-XXXX-XXXX"; // Replace locally. Do not commit real numbers.

void setup() {
  pinMode(exhaustFanPin, OUTPUT);
  pinMode(extinguisherPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(mq2SensorPin, INPUT);
  pinMode(flameSensorPin, INPUT);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  gsmSerial.begin(9600);
  Serial.begin(9600);

  lcd.print("FIRE & GAS");
  lcd.setCursor(0, 1);
  lcd.print("DETECTOR SYSTEM");
  delay(2000);
  lcd.clear();
}

void loop() {
  int gasValue = analogRead(mq2SensorPin);
  int flameValue = digitalRead(flameSensorPin); // LOW usually indicates flame detected

  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.print(" | Flame: ");
  Serial.println(flameValue == LOW ? "HIGH" : "OFF");

  if (gasValue > gasThreshold) {
    handleGasDetected(gasValue);
  } else if (flameValue == LOW) {
    handleFireDetected();
  } else {
    handleStable(gasValue);
  }

  delay(1500);
}

void handleGasDetected(int gasValue) {
  digitalWrite(exhaustFanPin, HIGH);
  digitalWrite(extinguisherPin, LOW);
  digitalWrite(buzzerPin, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAS DETECTED");
  lcd.setCursor(0, 1);
  lcd.print("GAS: ");
  lcd.print(gasValue);

  sendSms("SMOKE / GAS level is HIGH");
}

void handleFireDetected() {
  digitalWrite(exhaustFanPin, HIGH);
  digitalWrite(extinguisherPin, LOW);
  digitalWrite(buzzerPin, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FIRE DETECTED");
  lcd.setCursor(0, 1);
  lcd.print("FIRE: HIGH");

  callEmergencyContact();
}

void handleStable(int gasValue) {
  digitalWrite(exhaustFanPin, LOW);
  digitalWrite(extinguisherPin, HIGH);
  digitalWrite(buzzerPin, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM STABLE");
  lcd.setCursor(0, 1);
  lcd.print("GAS:");
  lcd.print(gasValue);
  lcd.print(" FIRE:OFF");
}

void sendSms(const char* message) {
  gsmSerial.println("AT+CMGF=1");
  delay(1000);

  gsmSerial.print("AT+CMGS="");
  gsmSerial.print(emergencyNumber);
  gsmSerial.println(""");
  delay(1000);

  gsmSerial.println(message);
  delay(100);
  gsmSerial.write(26); // CTRL+Z
  delay(1000);
}

void callEmergencyContact() {
  gsmSerial.print("ATD");
  gsmSerial.print(emergencyNumber);
  gsmSerial.println(";");
  delay(30000);
  gsmSerial.println("ATH");
}

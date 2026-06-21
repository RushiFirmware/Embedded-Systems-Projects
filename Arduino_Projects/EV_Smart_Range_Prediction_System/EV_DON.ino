#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int batteryPin = A0;
int distancePin = A1;

int ledPin = 7;
int buzzerPin = 8;

float fullVoltage = 48.0;
float dividerFactor = 11.0;
float energyPerKm = 1.5;

// Charging stations (km from start)
float station1 = 10;
float station2 = 25;
float station3 = 40;

void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("EV SMART SYS");
  delay(2000);
  lcd.clear();
}

void loop() {

  // -------- BATTERY READ --------
  int adcValue = analogRead(batteryPin);
  float vOut = adcValue * (5.0 / 1023.0);
  float batteryVoltage = vOut * dividerFactor;

  float soc = (batteryVoltage / fullVoltage) * 100.0;

  if (soc > 100) soc = 100;
  if (soc < 0) soc = 0;

  // -------- DISTANCE FROM POT --------
  int potValue = analogRead(distancePin);
  float distance = map(potValue, 0, 1023, 0, 100);

  // -------- REMAINING RANGE --------
  float remainingRange = soc / energyPerKm;

  // -------- LCD LINE 1 --------
  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(soc,0);
  lcd.print("% R:");
  lcd.print(remainingRange,0);
  lcd.print("   ");

  // -------- LCD LINE 2 --------
  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(distance,0);
  lcd.print(" ");

  if (batteryVoltage < 40.0) {

      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      lcd.print("LOW BATTERY ");
  }
  else if (remainingRange >= distance) {

      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      lcd.print("Safe        ");
  }
  else {

      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);

      if (remainingRange >= station1)
          lcd.print("Charge@10   ");
      else if (remainingRange >= station2)
          lcd.print("Charge@25   ");
      else if (remainingRange >= station3)
          lcd.print("Charge@40   ");
      else
          lcd.print("No Station  ");
  }

  delay(500);
}
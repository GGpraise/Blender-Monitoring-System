#include <LiquidCrystal.h>

// LCD pins
LiquidCrystal lcd(12, 11, 10, 9, 8, 4);

// Pins
const int tempPin = A0;
const int piezoPin = A1;
const int motorPin = 3;
const int buzzer = 2;

// Thresholds
float tempWarning = 35.0;
float tempDanger  = 50.0;

int vibrationWarning = 200;
int vibrationDanger  = 500;

// ADD THIS 👇
String state = "SAFE";

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("System Booting...");
  delay(2000);
  lcd.clear();

  pinMode(motorPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  analogWrite(motorPin, 200);
}

void loop() {

  // ===== READ TEMPERATURE =====
  int tempRaw = analogRead(tempPin);
  float voltage = tempRaw * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100;

  // ===== READ VIBRATION (FIXED) =====
  int vibration = analogRead(piezoPin);

  noTone(buzzer); // reset buzzer

  // ===== SYSTEM STATES =====

  if (temperature < tempWarning && vibration < vibrationWarning) {
    analogWrite(motorPin, 200);
    state = "SAFE";
  }

  else if ((temperature >= tempWarning && temperature < tempDanger) ||
           (vibration >= vibrationWarning && vibration < vibrationDanger)) {

    tone(buzzer, 1000);
    analogWrite(motorPin, 150);
    state = "WARNING";
  }

  else if (temperature >= tempDanger || vibration >= vibrationDanger) {

    tone(buzzer, 2000);
    delay(200);
    noTone(buzzer);
    delay(200);

    analogWrite(motorPin, 0);
    state = "DANGER";
  }

  // ===== LCD DISPLAY =====
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print("C   "); // spacing clears old text

  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(vibration);
  lcd.print(" ");
  lcd.print(state);
  lcd.print("   "); // clears leftovers

  // ===== SERIAL =====
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" | Vib: ");
  Serial.print(vibration);
  Serial.print(" | State: ");
  Serial.println(state);

  delay(500);
}

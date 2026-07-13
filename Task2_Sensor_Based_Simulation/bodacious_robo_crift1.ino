/*
 * Smart Home Temperature Monitoring & Cooling System
 * CodeAlpha IoT Internship - Task 2
 */

#include <LiquidCrystal.h>

// Pin Definitions
const int TEMP_SENSOR_PIN = A0;
const int GREEN_LED_PIN = 8;
const int RED_LED_PIN = 9;
const int BUZZER_PIN = 10;

// LCD Setup (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Temperature threshold
const float TEMP_THRESHOLD = 30.0;

// Variables
float currentTemperature = 0.0;
int sensorRawValue = 0;
float sensorVoltage = 0.0;

// Custom degree symbol
byte degreeSymbol[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  Serial.begin(9600);
  Serial.println("Smart Home Temperature Monitoring System");
  
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  lcd.begin(16, 2);
  lcd.createChar(0, degreeSymbol);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Smart Home");
  lcd.setCursor(0, 1);
  lcd.print("  Temp Monitor");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Status: ");
}

void loop() {
  readTemperature();
  updateLCDDisplay();
  controlOutputs();
  printToSerial();
  delay(1000);
}

void readTemperature() {
  sensorRawValue = analogRead(TEMP_SENSOR_PIN);
  sensorVoltage = (sensorRawValue / 1023.0) * 5.0;
  currentTemperature = (sensorVoltage - 0.5) / 0.01;
}

void updateLCDDisplay() {
  lcd.setCursor(6, 0);
  lcd.print(currentTemperature, 1);
  lcd.write(byte(0));
  lcd.print("C  ");
  
  lcd.setCursor(8, 1);
  if (currentTemperature >= TEMP_THRESHOLD) {
    lcd.print("HIGH TEMP!");
  } else {
    lcd.print("Normal    ");
  }
}

void controlOutputs() {
  if (currentTemperature >= TEMP_THRESHOLD) {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
  } else {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
}

void printToSerial() {
  Serial.print("Temp: ");
  Serial.print(currentTemperature, 1);
  Serial.print(" C | Status: ");
  
  if (currentTemperature >= TEMP_THRESHOLD) {
    Serial.println("HIGH TEMPERATURE ALERT!");
  } else {
    Serial.println("Normal");
  }
}
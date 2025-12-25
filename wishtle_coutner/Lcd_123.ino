/*
 * Whistle Counter with Arduino Uno
 * Components: Arduino Uno, I2C 16x2 LCD, Sound Sensor Module
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD with I2C address 0x27 (or 0x3F)
// Format: LiquidCrystal_I2C(address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sound sensor pin
const int soundPin = A0;

// Variables
int whistleCount = 0;
int soundThreshold = 500;  // Adjust based on your sensor (0-1023)
bool soundDetected = false;
unsigned long lastDetectionTime = 0;
const int debounceDelay = 500;  // Minimum time between detections (ms)

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize sound sensor pin
  pinMode(soundPin, INPUT);
  
  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Whistle Counter");
  lcd.setCursor(0, 1);
  lcd.print("Count: 0");
  
  // Initialize serial for debugging
  Serial.begin(9600);
  Serial.println("Whistle Counter Started");
  Serial.println("Calibrating...");
}

void loop() {
  // Read sound sensor value
  int soundValue = analogRead(soundPin);
  
  // Debug: print sound level
  Serial.print("Sound Level: ");
  Serial.println(soundValue);
  
  // Check if sound exceeds threshold
  if (soundValue > soundThreshold && !soundDetected) {
    // Check if enough time has passed since last detection
    if (millis() - lastDetectionTime > debounceDelay) {
      whistleCount++;
      soundDetected = true;
      lastDetectionTime = millis();
      
      // Update LCD
      lcd.setCursor(0, 1);
      lcd.print("Count: ");
      lcd.print(whistleCount);
      lcd.print("  ");  // Clear extra digits
      
      // Visual feedback
      lcd.setCursor(15, 0);
      lcd.print("*");
      
      Serial.print("Whistle detected! Count: ");
      Serial.println(whistleCount);
    }
  }
  
  // Reset detection flag when sound drops below threshold
  if (soundValue < soundThreshold) {
    soundDetected = false;
    lcd.setCursor(15, 0);
    lcd.print(" ");
  }
  
  delay(50);  // Small delay for stability
}

/*
 * TROUBLESHOOTING:
 * If LCD shows nothing:
 * 1. Try changing address from 0x27 to 0x3F in line 10
 * 2. Run I2C scanner sketch to find correct address
 * 
 * CALIBRATION INSTRUCTIONS:
 * 1. Open Serial Monitor (Tools > Serial Monitor, 9600 baud)
 * 2. Observe the "Sound Level" values in quiet environment
 * 3. Whistle and note the peak values
 * 4. Set soundThreshold to a value between quiet and whistle levels
 * 5. Typical values: 400-700 (adjust based on your sensor)
 * 
 * LIBRARY REQUIRED:
 * Install "LiquidCrystal I2C" by Frank de Brabander
 * (Arduino IDE: Sketch > Include Library > Manage Libraries)
 */
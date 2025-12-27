#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SOUND_PIN 2
#define LIGHT_PIN 8

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool lightState = false;
int clapCount = 0;
unsigned long firstClapTime = 0;

const unsigned long clapWindow = 700;   // time between claps
const unsigned long debounceTime = 200;

unsigned long lastSoundTime = 0;

void setup() {
  pinMode(SOUND_PIN, INPUT_PULLUP);
  pinMode(LIGHT_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Clap Activated");
  lcd.setCursor(0, 1);
  lcd.print("Light Control");

  delay(2000);
  lcd.clear();
}

void loop() {
  int sound = digitalRead(SOUND_PIN);

  if (sound == LOW && millis() - lastSoundTime > debounceTime) {
    lastSoundTime = millis();

    if (clapCount == 0) {
      firstClapTime = millis();
      clapCount = 1;
    } 
    else if (clapCount == 1 && millis() - firstClapTime <= clapWindow) {
      // DOUBLE CLAP DETECTED
      lightState = !lightState;
      digitalWrite(LIGHT_PIN, lightState ? HIGH : LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Light Status:");
      lcd.setCursor(0, 1);
      lcd.print(lightState ? "LIGHT ON " : "LIGHT OFF");

      clapCount = 0;
    }
  }

  // Reset if second clap not received
  if (clapCount == 1 && millis() - firstClapTime > clapWindow) {
    clapCount = 0;
  }
}

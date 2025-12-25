// Person Detection System using Ultrasonic Sensor
// Components Required:
// - Arduino (Uno/Nano)
// - HC-SR04 Ultrasonic Sensor
// - 16x2 LCD Display (I2C)
// - Buzzer
// - Connecting wires

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define TRIG_PIN 9       // Ultrasonic Trigger pin
#define ECHO_PIN 10      // Ultrasonic Echo pin
#define BUZZER_PIN 8     // Buzzer pin

// Detection threshold (in cm)
#define DETECTION_DISTANCE 100  // Detect if object/person within 100cm (1 meter)

// Initialize LCD (I2C address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

long duration;
int distance;
bool personDetected = false;

void setup() {
  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("Person Detection");
  lcd.setCursor(0, 1);
  lcd.print("Ultrasonic Mode");
  delay(2000);
  lcd.clear();
  
  // Initial display
  lcd.setCursor(0, 0);
  lcd.print("Status:");
  lcd.setCursor(0, 1);
  lcd.print("No Person");
  
  Serial.begin(9600);
}

void loop() {
  // Measure distance using ultrasonic sensor
  distance = measureDistance();
  
  // Print distance to Serial Monitor for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check if person is detected
  if (distance > 0 && distance <= DETECTION_DISTANCE) {
    // Person detected within range
    if (!personDetected) {
      // State changed to detected
      Serial.println("Person Detected!");
      
      // Update LCD
      lcd.setCursor(0, 1);
      lcd.print("Person Detected ");
      
      // Display distance on LCD
      lcd.setCursor(0, 1);
      lcd.print("Person: ");
      lcd.print(distance);
      lcd.print("cm   ");
      
      // Sound buzzer (beep pattern)
      for (int i = 0; i < 3; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(150);
        digitalWrite(BUZZER_PIN, LOW);
        delay(150);
      }
      
      personDetected = true;
    } else {
      // Continue displaying distance
      lcd.setCursor(0, 1);
      lcd.print("Person: ");
      lcd.print(distance);
      lcd.print("cm   ");
      
      // Continuous beep while person is detected
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    // No person detected
    if (personDetected) {
      // State changed to not detected
      Serial.println("No Person");
      
      // Update LCD
      lcd.setCursor(0, 1);
      lcd.print("No Person      ");
      
      // Turn off buzzer
      digitalWrite(BUZZER_PIN, LOW);
      
      personDetected = false;
    }
  }
  
  delay(200);  // Small delay between measurements
}

// Function to measure distance using HC-SR04
int measureDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send 10us pulse to trigger
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin
  duration = pulseIn(ECHO_PIN, HIGH, 30000);  // Timeout after 30ms
  
  // Calculate distance in cm (speed of sound = 343m/s)
  // Distance = (Time × Speed) / 2
  distance = duration * 0.034 / 2;
  
  // Return distance (0 if out of range or error)
  if (distance >= 400 || distance <= 0) {
    return 0;  // Out of range
  }
  return distance;
}

/*
===== CIRCUIT CONNECTIONS =====

Arduino Pin Connections:

1. ULTRASONIC SENSOR (HC-SR04):
   - VCC → Arduino 5V
   - TRIG → Arduino Digital Pin 9
   - ECHO → Arduino Digital Pin 10
   - GND → Arduino GND

2. LCD DISPLAY (16x2 I2C):
   - VCC → Arduino 5V
   - GND → Arduino GND
   - SDA → Arduino A4 (SDA)
   - SCL → Arduino A5 (SCL)

3. BUZZER:
   - Positive (+) → Arduino Digital Pin 8
   - Negative (-) → Arduino GND

4. POWER:
   - Arduino powered via USB or external 7-12V adapter

===== CIRCUIT DIAGRAM (Text) =====

                    +5V
                     |
        +------------+------------+------------+
        |            |            |            |
   [US VCC]     [LCD VCC]    [Arduino]        |
        |            |            |            |
   [HC-SR04]    [LCD I2C]        |            |
   Ultrasonic       |            |            |
    Sensor           |            |            |
        |          SDA-----------A4           |
   TRIG--------------->Pin 9     |            |
   ECHO--------------->Pin 10  SCL-----------A5
        |            |            |            |
      [GND]      [LCD GND]    Pin 8----[Buzzer+]
        |            |            |         |
        +------------+------------+-----[Buzzer-]
                     |
                    GND

===== CIRCUIT DIAGRAM (Visual) =====

        HC-SR04
    +--------------+
    | VCC TRIG ECHO GND |
    +--+----+----+--+
       |    |    |  |
       |    |    |  |
    +--+----+----+--+----------------+
    | 5V   9   10 GND    A4  A5   8  |  Arduino
    +------+--------+---------+------+
           |        |         |      |
           |      [LCD]       |   [Buzzer]
           |     (I2C)        |
           +--SDA  SCL--+     +--[+][-]
                        |
                       GND

===== NOTES =====
- Detection range: 2cm to 400cm (adjustable in code)
- Current setting: detects person within 100cm (1 meter)
- Change DETECTION_DISTANCE to adjust sensitivity
- Ultrasonic sensor works best with flat surfaces
- Angle of detection: approximately 15 degrees cone
- For better accuracy, keep sensor stable and level
- Distance is displayed on LCD in centimeters
- I2C LCD address might be 0x3F (check with I2C scanner if needed)

===== TROUBLESHOOTING =====
- If LCD shows nothing: Check I2C address (use I2C scanner sketch)
- If distance always 0: Check TRIG and ECHO connections
- If too sensitive: Increase DETECTION_DISTANCE value
- If not sensitive enough: Decrease DETECTION_DISTANCE value
*/
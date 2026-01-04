#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // Ensure you have the I2C LCD Library

// Pin Definitions
#define LEFT_FLAME 2
#define RIGHT_FLAME 3
#define MQ2_PIN A0
#define BUZZER_PIN 8
#define SERVO_PIN 9

// Threshold for Gas Detection
int gasThreshold = 400; 

Servo servoMotor;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address to 0x3F if 0x27 doesn't work with your LCD display.

void setup() {
  pinMode(LEFT_FLAME, INPUT);
  pinMode(RIGHT_FLAME, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(90); // Start Centered
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Loading..");
  delay(2000);
  lcd.clear();
}

void loop() {
  int leftState = digitalRead(LEFT_FLAME);
  int rightState = digitalRead(RIGHT_FLAME);
  int gasValue = analogRead(MQ2_PIN);

  // --- GAS DETECTION LOGIC ---
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(gasValue);
  
  if (gasValue > gasThreshold) {
    lcd.print(" ALERT!");
    digitalWrite(BUZZER_PIN, HIGH); 
  } else {
    lcd.print(" Clean ");
    digitalWrite(BUZZER_PIN, LOW);
  }

  lcd.setCursor(0, 1);
  if (leftState == LOW && rightState == HIGH) {
    lcd.print("Flame: LEFT    ");
    servoMotor.write(0);
    digitalWrite(BUZZER_PIN, HIGH); 
  } 
  else if (rightState == LOW && leftState == HIGH) {
    lcd.print("Flame: RIGHT   ");
    servoMotor.write(180);
    digitalWrite(BUZZER_PIN, HIGH);
  } 
  else if (leftState == LOW && rightState == LOW) {
    lcd.print("Flame: CENTER  ");
    servoMotor.write(90);
    digitalWrite(BUZZER_PIN, HIGH);
  } 
  else {
    lcd.print("Flame: NONE    ");
    // Servo stays at last position or returns to center
    if (gasValue <= gasThreshold) digitalWrite(BUZZER_PIN, LOW);
  }

  delay(200); // Small delay for stability
}

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; // Row pins connected to pins 4, 5, 6, 7
byte colPins[COLS] = {8, 9, 10, 11}; // Column pins connected to pins 8, 9, 10, 11

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Servo configuration
Servo myServo;
const int servoPin = 10;
const int lockPosition = 0;    // Locked position
const int unlockPosition = 90; // Unlocked position

// Password configuration
String password = "1234";
String inputPassword = "";

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  myServo.attach(servoPin);
  myServo.write(lockPosition); // Initially lock the door
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
  lcd.setCursor(0, 1);
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    if (key == '#') {
      if (inputPassword == password) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access granted");
        myServo.write(unlockPosition); // Unlock the door
        delay(5000); // Keep the door unlocked for 5 seconds
        myServo.write(lockPosition); // Lock the door again
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter password:");
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access denied");
        delay(2000); // Display message for 2 seconds
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter password:");
      }
      inputPassword = ""; // Reset the input password
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear the second line
    } else if (key == '*') {
      if (inputPassword.length() > 0) {
        inputPassword.remove(inputPassword.length() - 1); // Remove the last character
        lcd.setCursor(0, 1);
        lcd.print(inputPassword);
        lcd.print(" "); // Clear the last character position
      }
    } else {
      inputPassword += key; // Append the key to the input password
      lcd.setCursor(0, 1);
      lcd.print(inputPassword);
    }
  }
}

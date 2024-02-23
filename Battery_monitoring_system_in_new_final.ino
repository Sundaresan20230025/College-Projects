#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27 for a 16x2 display
int relayPin = 2; // Pin connected to relay
int batteryPin = A0; // Analog pin connected to battery monitoring module
float batteryVoltage = 0.0; // Variable to store battery voltage

void setup() {
  pinMode(relayPin, OUTPUT);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.print("Smart Charger");
}

void loop() {
  // Read battery voltage
  int sensorValue = analogRead(batteryPin);
  batteryVoltage = sensorValue * (12.6 / 1023.0); // Convert analog reading to voltage

  // Display battery voltage
  lcd.setCursor(0, 1);
  lcd.print("Battery: ");
  lcd.print(batteryVoltage);
  lcd.print("V");

  // Check battery voltage and control relay accordingly
  if (batteryVoltage <2.0) { // Example threshold, adjust as needed
    digitalWrite(relayPin, HIGH); // Turn on charging
    lcd.setCursor(0, 0);
    lcd.print("Charging: ON ");
  } else {
    digitalWrite(relayPin, LOW); // Turn off charging
    lcd.setCursor(0, 0);
    lcd.print("Charging: OFF");
  }

  delay(10000); // Delay for stability
}

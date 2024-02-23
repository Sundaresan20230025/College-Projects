#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

const int trigPin = 5;         
const int echoPin = 3;  

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 7
long duration;
int distance;
LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522 card
  pinMode(BUZZER_PIN, OUTPUT);  // Set buzzer pin as an output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("Home Security");
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance= duration * 0.034 / 2; // Calculate distance in cm
    lcd.setCursor(0, 1);
    lcd.print("Dis: ");
    lcd.println(distance);
 
  // Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  Serial.println(content.substring(1));
  
  if (content.substring(1) == "73 CE 62 DD") { // Change XX XX XX XX to your authorized tag's UID
    lcd.setCursor(0, 0);
    lcd.println("Authorized access");
    // Turn on relay/LED for access
  } else {
    lcd.setCursor(0, 0);
    lcd.println("  Unauthorized  ");
    digitalWrite(BUZZER_PIN, HIGH); // Activate the buzzer for unauthorized access
    lcd.setCursor(8, 1);
    lcd.print(" Buz:");
    lcd.print(" on");
    delay(1000); // Wait for 1 second
    digitalWrite(BUZZER_PIN, LOW); // Deactivate the buzzer
    lcd.setCursor(8, 1);
    lcd.print("Buz:");
    lcd.print(" off");
  
  }
}

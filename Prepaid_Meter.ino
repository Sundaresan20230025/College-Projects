// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int lastmillis;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display
float kwh;
unsigned long totalET;
float dailykwh;
float totalkwh;
int timer = 15;
int cardid = 0;
void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  emon1.voltage(A0, 149, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(A1, 25);       // Current: input pin, calibration.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop() {
  emon1.calcVI(20, 2000);        // Calculate all. No.of half wavelengths (crossings), time-out
  float realPower       = emon1.realPower * -1;      //extract Real Power into variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable
  Serial.println("V: " + String(supplyVoltage));
  Serial.println("I: " + String(Irms));
  Serial.println("P: " + String(realPower));
  lcd.setCursor(0, 0);  //Set cursor to character 2 on line 0
  lcd.print("V:" + String(supplyVoltage));
  lcd.setCursor(7, 0);  //Set cursor to character 2 on line 0
  lcd.print(" I:" + String(Irms));
  lcd.setCursor(0, 1);  //Set cursor to character 2 on line 0
  lcd.print("P:" + String(realPower));
  uint32_t ts1 = millis();
  delay(1000); //reports at 1 second intervals
  uint32_t ts2 = millis();
  // print the time interval in seconds
  uint32_t ts3 = (ts2 - ts1) / 1000;
  totalET = totalET + ts3;
  Serial.print("Seconds: ");
  Serial.println(totalET);
  kwh = realPower * ts3 / 3600000;
  dailykwh = dailykwh + kwh;
  totalkwh = totalkwh + dailykwh;
  Serial.print("Total KWh: "); //from reboot
  Serial.println(totalkwh, 4);
  lcd.setCursor(7, 1);  //Set cursor to character 2 on line 0
  lcd.print(" KWh:" + String(totalkwh));

  Serial.println(cardid);
  Serial.println(timer);
  if (totalET >= timer && cardid == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("Need Topup :(");//from reboot
    Serial.println("Please Recharge");
    delay(2000);
  }
  if (totalET >= timer && cardid == 1) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("Need Topup :(");//from reboot
    Serial.println("Please Recharge");
    delay(2000);
  }
  if (totalET >= timer && cardid == 2) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("Need Topup :(");//from reboot
    Serial.println("Please Recharge");
    delay(2000);
  }
  if (totalET >= 86400) { //restart every 24 hours
    ts1 = 0;
    ts2 = 0;
    totalET = 0;
    dailykwh = 0;
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  Serial.println(content.substring(1));
  ts1 = 0;
  ts2 = 0;
  totalET = 0;
  dailykwh = 0;
  if (content.substring(1) == "13 A6 B8 00") //change here the UID of the card/cards that you want to give access
  {
    cardid = 1;
    timer = 30;
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("Reacharged :)");//from reboot
    delay(2000);
  }
  else if (content.substring(1) == "33 34 5E DD") //change here the UID of the card/cards that you want to give access
  {
    cardid = 2;
    timer = 60;
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("Reacharged :)");//from reboot
    delay(2000);
  }
  else   {
    cardid = 0;
    timer = 15;
  }
}

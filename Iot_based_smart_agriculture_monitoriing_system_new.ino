#define BLYNK_TEMPLATE_ID "TMPL3GyHlzA-0"
#define BLYNK_TEMPLATE_NAME "IOT Based smart agriculture monitoring system"
#define BLYNK_AUTH_TOKEN "F6XNOvnqZwfqeoajo-1MAYK2TisIoPBA"
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const int dhtPin = D4;   // Digital pin for DHT11 sensor          
const int moisture_sensor = A0;  // A0
const int trigPin = D0;         
const int echoPin = D7;        
const int relay = D3;         // D3
String status_pump = "";
// Initialize the DHT sensor
DHT dht(dhtPin, DHT11);

#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
LiquidCrystal_I2C lcd(0x27, 16, 2);
char ssid[] = "vetri";
char pass[] = "9842878776";
long duration;
float distanceCm;
float distanceInch;
void  setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("Smart Agri Unit");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  delay(2000);
}
void loop()
{
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int M = analogRead(moisture_sensor);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float WL = duration * 0.034 / 2; // Calculate distance in cm

  if (M < 900) {
    digitalWrite(relay, LOW);
    status_pump = "Pump Off";
    Serial.println("Pump Off");
  }
  else {
    digitalWrite(relay, HIGH);
    Serial.println("Pump On");
    status_pump = "Pump On";
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Hum: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Moisture: ");
  Serial.println(M);

  Serial.print("Distance: ");
  Serial.print(WL);
  Serial.println(" cm");

  Serial.print("Pump Status: ");
  Serial.println(status_pump);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(hum);
  lcd.print(" %");

  lcd.setCursor(9, 0);
  lcd.print("M: ");
  lcd.print(M);

  lcd.setCursor(9, 1);
  lcd.print("D: ");
  lcd.print(WL);
  lcd.print(" cm");

  
  

  Blynk.virtualWrite(V0, hum);
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, M);
  Blynk.virtualWrite(V3, WL);
  Blynk.virtualWrite(V4, status_pump);

  delay(2000);

  
}

#define BLYNK_TEMPLATE_ID "TMPL30cVBVWBH"
#define BLYNK_TEMPLATE_NAME "IOT HOME AUTOMATION"
#define BLYNK_AUTH_TOKEN "BR5A3qNRGyQEW6uPKSazCJ1J9ei4__gl"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "IDEA";  // type your wifi name
char pass[] = "12345678";  // type your wifi password

int relaypin1 = D1; // Connect to Pin D1 in NodeMCU
int relaypin2 = D2; // Connect to Pin D2 in NodeMCU
int relaypin3 = D3; // Connect to Pin D3 in NodeMCU
int relaypin4 = D4; // Connect to Pin D4 in NodeMCU

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);
  pinMode(relaypin3, OUTPUT);
  pinMode(relaypin4, OUTPUT);

  // You can add more initialization code here
}

BLYNK_WRITE(V1) {
  int value = param.asInt(); // Get the value from the virtual pin
  digitalWrite(relaypin1, value); // Set the state of relaypin1 based on the value
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  digitalWrite(relaypin2, value);
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  digitalWrite(relaypin3, value);
}

BLYNK_WRITE(V4) {
  int value = param.asInt();
  digitalWrite(relaypin4, value);
}

void loop()
{
  Blynk.run();
  // You can add other non-blocking code here if needed
}

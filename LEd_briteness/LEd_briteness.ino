#include"arduino.h"
#define LED_PIN 9
#define POTENTIOMETER_PIN A4
void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode (13, OUTPUT);
  pinMode (A4, INPUT);
  Serial.begin(9600);
  
}
void loop()
{
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int brightness = potentiometerValue / 4;
  analogWrite(LED_PIN, brightness);
delay(10000);
 Serial.println  (A5);
  int potValue = analogRead (A5);
  digitalWrite (13, HIGH);
  delay (potValue);
  digitalWrite (13, LOW);
  delay (potValue);
}

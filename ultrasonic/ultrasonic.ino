
#include "Arduino.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int trigPin = 8;
const int echoPin = 9;

#define SOUND_SPEED 0.034

long duration;
float distanceCm;
void setup()
{
 
 lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  ThingSpeak.begin(client);
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delay(200);
  digitalWrite(trigPin, HIGH);
  delay(200);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2; //formula

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  delay(200);


  delay(10000);
}

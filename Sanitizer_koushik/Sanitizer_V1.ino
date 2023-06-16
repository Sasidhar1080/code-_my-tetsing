#include "Arduino.h"


const int trigPin1 = 35;
const int echoPin1 = 34;


long duration1;
int distance1;
void setup()
{
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(14,OUTPUT);
  Serial.begin(115200);



}
void loop()
{


  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(50);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.println(distance1);
  if (distance1 <= 15)
  {
    digitalWrite(14,HIGH);
    delay(200);
    digitalWrite(14,LOW);
    delay(3000);


  }
  else
  {

    digitalWrite(14,LOW);


  }

}

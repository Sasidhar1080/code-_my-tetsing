#include "Arduino.h"
#include "ThingSpeak.h"
#include<Wifi.h>
const int trigPin1 = 35;
const int echoPin1 = 34;
#define redled 32
#define grnled 33
unsigned long ch_no = 1504407;//Replace with Thingspeak Channel number
const char * write_api = "1X1AK0AQ0UUM1U8P";//Replace with Thingspeak write API
char auth[] = "mwa0000023187883";
char ssid[] = "DESKTOP-S8KH4G2 4224";
char pass[] = "88888888";
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 600000;
WiFiClient  client;
long duration1;
int distance1;
void setup()
{
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  digitalWrite(redled, LOW);
  digitalWrite(grnled, LOW);
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
    digitalWrite(14,LOW);
    delay(300);
    digitalWrite(14, HIGH);
    delay(500);

  }
  else
  {

    digitalWrite(14, HIGH);
    delay(500);
  }

}

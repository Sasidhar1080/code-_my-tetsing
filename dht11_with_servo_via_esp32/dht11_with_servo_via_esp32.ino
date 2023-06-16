#include "DHT.h" 
#include <Servo.h>
Servo myservo;  
int pos = 20; 
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  myservo.attach(13);
  pinMode(13,OUTPUT);
  pinMode(4,INPUT);
  Serial.begin(9600);
  Serial.println(F("DHT test!"));
  dht.begin();
  myservo.write(pos);

}
void loop() {
  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
  Serial.println(F("Failed to read from DHT sensor!"));
  return;
  }
// Compute heat index in Fahrenheit (the default)
   float hif = dht.computeHeatIndex(f, h);
// Compute heat index in Celsius
  float hic = dht.computeHeatIndex(t, h, false);
Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  if (t >= 25.00 || h >= 46.00)
  {
    //digitalWrite(led,HIGH);
    myservo.write(pos+100);
    delay(1000);
  }
  else if  (t >= 35.00 || h >= 56.00)
  {
    //digitalWrite(led,HIGH);
    myservo.write(pos+160);
    delay(1000);
  }
  else 
  {
   // digitalWrite(led,LOW);
      myservo.write(pos);
  }
  delay(300);

  
}

#include<Arduino.h>

int relayPin = 17  ;

void setup() {    
  Serial.begin(115200);
  delay(1000);
  pinMode(relayPin,OUTPUT);

 

}

void loop() {
   digitalWrite(relayPin,HIGH);
  Serial.println("ON");
  delay(5000);
  
  digitalWrite(relayPin, LOW);
  Serial.println("OFF");
  delay(5000);
  
}


#include <WiFi.h>
#include "ThingSpeak.h"
const int analogInPin =14;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
const char* ssid = "SCRC_LAB";   // your network SSID (name) 
const char* password = "Scrciiith@123";
WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "XXXXXXXXXXXXXXXX";

void setup() {

  
  
  Serial.begin(9600);
    WiFi.mode(WIFI_STA);  
  
  ThingSpeak.begin(client);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  Serial.print("sensor = ");
  Serial.println(sensorValue);


  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}

#include "WiFi.h"
#include <Arduino.h>
#include "ThingSpeak.h"
#include <WiFiClient.h>
#include <WiFiAP.h>

// ThingSpeak Channel ID and Api Keys
#define CHANNEL_ID 1654489
#define CHANNEL_API_KEY "HF9EZ1SU90JTPI92"







WiFiClient client;

// Wifi Details
const char *ssid = "SCRC_LAB";
const char *password = "Scrciiith@123";

//const char *ssid = "realme 5";
//const char *password = "12345987";

//const char *ssid = "Spidy Sasidhar";
//const char *password = "10.10.10.10";

void setup()
{

  Serial.begin(9600);
  WiFi.localIP();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop()
{
  int sensorValue = analogRead(34);// read the input on analog pin 0:
  float voltage = sensorValue * (3.3/ 4095.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage); // print out the value you read:
  delay(500);
  
  //printing values
  Serial.print("voltage: ");
  Serial.println(voltage );
 
  //delay(200);


  ThingSpeak.writeField(CHANNEL_ID, 1 , voltage , CHANNEL_API_KEY);
 
  delay(10000);
}

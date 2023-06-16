#include "WiFi.h"
#include <Arduino.h>
#include "ThingSpeak.h"
#include <WiFiClient.h>
#include <WiFiAP.h>

  // ThingSpeak Channel ID and Api Keys
  #define CHANNEL_ID 1703829
  #define CHANNEL_API_KEY "RPIUID9T59UDW4IF"


  int tdsValue = 60;
  float Temp = 25;
  float turbudity = 3.3;
  float level= 12;
  float pH =5.9;
    WiFiClient client;
  
  // Wifi Details
  
  const char *ssid = "SCRC_LAB";
  const char *password = "Scrciiith@123";
  
void setup(){
        Serial.begin(115200);
      
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

  void loop(){
    ThingSpeak.writeField(CHANNEL_ID, 1 , tdsValue, CHANNEL_API_KEY);
    ThingSpeak.writeField(CHANNEL_ID, 2 , Temp, CHANNEL_API_KEY);
    ThingSpeak.writeField(CHANNEL_ID, 3 , pH, CHANNEL_API_KEY);
    ThingSpeak.writeField(CHANNEL_ID, 4 , turbudity, CHANNEL_API_KEY);
    ThingSpeak.writeField(CHANNEL_ID, 5 , level, CHANNEL_API_KEY); 
    delay(500);
    
    }

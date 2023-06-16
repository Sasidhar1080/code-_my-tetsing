
 
  #include "WiFi.h"
  #include <Arduino.h>
  #include "ThingSpeak.h"
  #include <WiFiClient.h>
  #include <WiFiAP.h>
  #include "OneWire.h"
  #include "DallasTemperature.h"
 

  
  // ThingSpeak Channel ID and Api Keys
  #define CHANNEL_ID 1938355
  #define CHANNEL_API_KEY "O2ZKUI7KT7ESRW8J"
  
  // tds And temp
  
  OneWire oneWire(27);
  DallasTemperature tempSensor(&oneWire);
  #define tdssensorPin 35
  
 
  
  const int potPin=27;
  
  int tdssensorValue = 0;
  float tdsValue = 0;
  float Voltage = 0;
  float Temp=0;
  
  WiFiClient client;
  
  // Wifi Details
  
  const char *ssid = "Spidy Sasi";
  const char *password = "10.10.0.1";
  
  void setup()
  {


           
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


      
        tempSensor.begin();
        ThingSpeak.begin(client);
  }
  
  void loop()
  {
  
  
    //sensors data 

    //temp and tds value
      tdssensorValue = analogRead(tdssensorPin);
      Voltage = tdssensorValue*3.3/1024.0; //Convert analog reading to Voltage
//      Serial.print(Voltage);
      tdsValue=(133.42*Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
      Serial.print("TDS Value = ");
      Serial.print(tdsValue);
      Serial.println(" ppm");
     tempSensor.requestTemperaturesByIndex(0);
     
    Temp=tempSensor.getTempCByIndex(0); 
    
    Serial.print("Temperature: ");
    Serial.print(Temp);
    Serial.println(" C");

  //thingspeak 

    ThingSpeak.setField(1,tdsValue);
    ThingSpeak.setField(2,Temp);


    ThingSpeak.writeFields(CHANNEL_ID,CHANNEL_API_KEY);

    delay (1000);

    

   
  }

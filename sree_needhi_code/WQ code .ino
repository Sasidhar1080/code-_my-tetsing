  #include "WiFi.h"
  #include <Arduino.h>
  #include "ThingSpeak.h"
  #include <WiFiClient.h>
  #include <WiFiAP.h>
  #include "OneWire.h"
  #include "DallasTemperature.h"
  #include <Adafruit_SH1106.h>
  #include <Adafruit_GFX.h>

  
  // ThingSpeak Channel ID and Api Keys
  #define CHANNEL_ID 1703829
  #define CHANNEL_API_KEY "RPIUID9T59UDW4IF"
  
  // tds And temp
  
  OneWire oneWire(27);
  DallasTemperature tempSensor(&oneWire);
  #define tdssensorPin 35
  
  //PH pin 
  
  const int potPin=14;
  
  int tdssensorValue = 0;
  float tdsValue = 0;
  float Voltage = 0;
  float Temp=0;
  float turbudity =0;
  float ph;
  float Value=0;
  
  // motor pins
  int pump1 = 18 ;  
  int pump2 = 19 ; 

  // ultrasonic pins
  const int trigPin = 16;
  const int echoPin = 17;
  
  //define sound speed in cm/uS
  #define SOUND_SPEED 0.034
  #define totallevel 10
  
  long duration;
  float distanceCm;
  long levelValue ;
 
  // oled pins

  #define OLED_SDA 21
  #define OLED_SCL 22
  
  Adafruit_SH1106 display(21, 22);
  
  #define NUMFLAKES 10
  #define XPOS 0
  #define YPOS 1
  #define DELTAY 2
  
  #if (SH1106_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SH1106.h!");
  #endif
  
  WiFiClient client;
  
  // Wifi Details
  
  const char *ssid = "SCRC_LAB";
  const char *password = "Scrciiith@123";
  
  void setup()
  {
      pinMode(pump1, OUTPUT);
      pinMode(pump2, OUTPUT);
      
      pinMode(trigPin, OUTPUT); 
      pinMode(echoPin, INPUT); 

      pinMode(OLED_SDA, INPUT_PULLUP);           
      pinMode(OLED_SCL, INPUT_PULLUP);          
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

        display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
        display.display();
        delay(1000);
        
        tempSensor.begin();
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
    Serial.println(distanceCm);
    
    levelValue=totallevel-distanceCm;

    
  //cleaning water before to get data 
     digitalWrite(pump2, LOW);      //Switch Solenoid ON
   delay(1000);
   Serial.println("motor 2 high");//Wait 1 Second
   delay(1000);                    //Wait 1 Second
   digitalWrite(pump2, HIGH);     //Switch Solenoid OFF Serial.println("hi");
   Serial.println("motor 2 low");//Wait 1 Second
   delay(1000); 
   
    //Getting water in to tank 
    
    digitalWrite(pump1,LOW);
    Serial.println("motor 1 high");//Switch Solenoid ON
    delay(1000);                          //Wait 1 Second
    digitalWrite(pump1, HIGH);
    Serial.println("motor 1 low");//Switch Solenoid OFF
    delay(1000);  
  
    //sensors data 
    
    //turbudity value
   
    int sensorValue = analogRead(34);// read the input on analog pin 0:
    float voltage = sensorValue * (3.3/ 4095.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    turbudity = voltage;
    Serial.print("turbudity voltage: ");
    Serial.print(turbudity);
    Serial.println("V");
    
    //temp and tds value
      tdssensorValue = analogRead(tdssensorPin);
      Voltage = tdssensorValue*3.3/1024.0; //Convert analog reading to Voltage
      tdsValue=(133.42*Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
      Serial.print("TDS Value = ");  
      Serial.print(tdsValue);
      Serial.println(" ppm");
     tempSensor.requestTemperaturesByIndex(0);
     
    Temp=tempSensor.getTempCByIndex(0); 
    
    Serial.print("Temperature: ");
    Serial.print(Temp);
    Serial.println(" C");
    
   // ph value
   
    Value= analogRead(potPin);   
//    float volt=Value*(3.3/4095.0);
    ph=(3.3*voltage);
    
    Serial.print("pH Vlaue:");
    Serial.println(ph);
    delay(500);
    
//level value 
    Serial.print("level=");
    Serial.print(levelValue);
    Serial.println("cm");
  
   // cleaning tank 
    digitalWrite(pump2, LOW);      //Switch Solenoid ON
   delay(1000);
   Serial.println("motor 2 high");//Wait 1 Second
   delay(1000);                    //Wait 1 Second
   digitalWrite(pump2, HIGH);     //Switch Solenoid OFF Serial.println("hi");
   Serial.println("motor 2 low");//Wait 1 Second
   delay(1000); 


  //thingspeak 

    ThingSpeak.setField(1,tdsValue);
    ThingSpeak.setField(2,Temp);
    ThingSpeak.setField(3,ph);
    ThingSpeak.setField(4,turbudity);
    ThingSpeak.setField(5,levelValue);

    ThingSpeak.writeFields(CHANNEL_ID,CHANNEL_API_KEY);

    delay (1000);

    
  //oled display
      
        // display node name in round rect
    display.setCursor(0,0);
    display.fillRoundRect(0, 2, 128, 64, 8, BLACK);
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(6,10);
    display.println("Water Quality");
    display.display();
    delay(1000);

 //level
  display.clearDisplay();

  display.setCursor(25,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("level:");

  display.setCursor(20,40);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println(levelValue);

  display.setCursor(90,40);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("cm");

  display.display();
  display.clearDisplay();

  delay(1000); 

 

  // pH

    display.clearDisplay();


    display.setCursor(32,0);
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.println("pH");

    display.setCursor(32,32);
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.println(ph);
    
    display.display();
    delay(1000);


  // Turbidity
    display.clearDisplay();

  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Turbidity:");

  display.setCursor(20,40);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println(turbudity);

  display.setCursor(75,40);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("V");

  display.display();
  delay(1000);

  
  // Temperarture
  display.clearDisplay();

  display.setCursor(32,0);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.println("Temp:");

  display.setCursor(10,32);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println(Temp);

  display.setCursor(75,32);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("°C");

  display.display();
  delay(1000);
  
// TDS

    display.clearDisplay();


    display.setCursor(32,0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("TDS:");

    display.setCursor(10,22);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println(tdsValue);

    display.setCursor(50,45);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("PPM");

    display.display();
    delay(1000);
   
  }

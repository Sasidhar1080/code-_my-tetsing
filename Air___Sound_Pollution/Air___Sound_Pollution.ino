#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <Adafruit_AHTX0.h>
#include <Adafruit_Sensor.h> // always include thingspeak header file after other header files and custom macros
#include <SDS011.h>
#include <SPI.h>
#define SOUND_SENSOR_PIN A0 // Analog input pin connected to sound level meter
#define CALIBRATION_FACTOR 1.2 // Scaling factor based on calibration

float p10,p25;
int error;
SDS011 my_sds;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels 

char ssid[] = "JioFi3_259D0E";   // your network SSID (name) 
char pass[] = "k61m12m0u5";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 2064926;
const char * myWriteAPIKey = "NRW04J0AX6ETM0S2";

int number = 0;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to hold temperature readings
float temperatureC;
//uncomment if you want to get temperature in Fahrenheit
//float temperatureF;

// Create a sensor object
Adafruit_AHTX0 aht; //BME280 connect to ESP32 I2C (GPIO 21 = SDA, GPIO 22 = SCL)

void initAHT(){
  if (!aht.begin()) {
    Serial.println("Could not find a valid AHT sensor, check wiring!");
    while (1);
  }
}

// Initialize our values
int t;
int h;
String myStatus = "";

void setup() {
  Serial.begin(115200);
  initAHT();  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  my_sds.begin(D5,D6);
  
  WiFi.mode(WIFI_STA);  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  int analogValue = analogRead(SOUND_SENSOR_PIN); // Read analog voltage from sound level meter
  float soundLevel = 20 * log10(analogValue / CALIBRATION_FACTOR); // Convert to dB using calibrated factor
  Serial.println(soundLevel); // Print sound level to serial monitor

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

// Get a new temperature reading
    sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  t = temp.temperature;
  h = humidity.relative_humidity;// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); 
  Serial.print(t); 
  Serial.println(" degrees C");
  Serial.print("Humidity: "); 
  Serial.print(h); 
  Serial.println("% rH");
  
  error = my_sds.read(&p25,&p10);
  if (! error) {
    Serial.println("P2.5: "+String(p25));
    Serial.println("P10:  "+String(p10));
  }
  delay(100);

  // set the fields with the values
  ThingSpeak.setField(1, temp.temperature);
  ThingSpeak.setField(2, humidity.relative_humidity);
  ThingSpeak.setField(3, p10);
  ThingSpeak.setField(4, p25);
  ThingSpeak.setField(5, soundLevel);
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(2064926, "NRW04J0AX6ETM0S2");
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000); // Wait 20 seconds to update the channel again
}

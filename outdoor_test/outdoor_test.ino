#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_AHTX0.h>
#include <Adafruit_Sensor.h>
#include <SDS011.h>
#include <SPI.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h> // Added WiFiClientSecure library for secure connection
#include "time.h"

#define SOUND_SENSOR_PIN A0
#define CALIBRATION_FACTOR 1.2

float p10, p25;
int error;
SDS011 my_sds;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

char ssid[] = "JioFi3_259D0E";
char pass[] = "k61m12m0u5";
int keyIndex = 0;

WiFiClientSecure client;

unsigned long myChannelNumber = 2064926;
const char *myWriteAPIKey = "NRW04J0AX6ETM0S2";
// onem2m code
#define CSE_IP "onem2m.iiit.ac.in" 
#define CSE_PORT 443
#define HTTPS true
#define OM2M_ORGIN "AirPoll@20:22uHt@Sas"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "AE-AQ"
#define OM2M_DATA_CONT "AQ-BN00-00/Data"

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to hold temperature readings
float temperatureC;

// Create a sensor object
Adafruit_AHTX0 aht;

void initAHT() {
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
  initAHT();
  while (!Serial) {
    ;
  }
  my_sds.begin(D5, D6);

  WiFi.mode(WIFI_STA);

  // Connect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Set time using NTP
  configTime(0, 0, "pool.ntp.org");
  Serial.println("Waiting for time sync...");
  while (time(nullptr) < 1000000000) {
    delay(1000);
  }
  Serial.println("Time synced!");

  // Set up the SSL client
  client.setInsecure(); // Ignore SSL verification

  ThingSpeak.begin(client);
}

void loop() {
  int analogValue = analogRead(SOUND_SENSOR_PIN);
  float soundLevel = 20 * log10(analogValue / CALIBRATION_FACTOR);
  Serial.println(soundLevel);

  // Get a new temperature reading
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  t = temp.temperature;
  h = humidity.relative_humidity;
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" degrees C");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("% rH");

  error = my_sds.read(&p25, &p10);
  if (!error) {
    Serial.println("P2.5: " + String(p25));
    Serial.println("P10:  " + String(p10));
  }
  delay(100);

  // Set the fields with the values
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, p10);
  ThingSpeak.setField(4, p25);
  ThingSpeak.setField(5, soundLevel);
  // Set the status
  ThingSpeak.setStatus(myStatus);

  // Write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Make a POST request to the CSE
  String data = "<?xml version='1.0' encoding='UTF-8'?>";
  data += "<m2m:cin xmlns:m2m='http://www.onem2m.org/xml/protocols'>";
  data += "<cnf>message</cnf>";
  data += "<con>";
  data += "<Temperature>" + String(t) + "</Temperature>";
  data += "<Humidity>" + String(h) + "</Humidity>";
  data += "<P10>" + String(p10) + "</P10>";
  data += "<P25>" + String(p25) + "</P25>";
  data += "<SoundLevel>" + String(soundLevel) + "</SoundLevel>";
  data += "</con>";
  data += "</m2m:cin>";

  String url = "http";
  if (HTTPS) {
    url += "s";
  }
  url += "://" + String(CSE_IP) + ":" + String(CSE_PORT) + String(OM2M_MN) + String(OM2M_AE) + "/" + String(OM2M_DATA_CONT);

  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: " + String(CSE_IP));
  client.println("X-M2M-Origin: " + String(OM2M_ORGIN));
  client.println("Content-Type: application/xml;ty=4");
  client.println("Connection: close");
  client.println("Content-Length: " + String(data.length()));
  client.println();
  client.println(data);

  delay(1000);

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.stop();

  delay(20000);
}

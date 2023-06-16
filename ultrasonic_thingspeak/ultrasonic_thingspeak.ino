#include "WiFi.h"
#include <Arduino.h>
#include "ThingSpeak.h"
#include <WiFiClient.h>
#include <WiFiAP.h>

// ThingSpeak Channel ID and Api Keys
#define CHANNEL_ID 1703824
#define CHANNEL_API_KEY "0O1S3FQGD2V3AXWC"

// Pins for the ESP 32
const int trigPin = 16;
const int echoPin = 17;

// constant values for converting
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// variables of the
long duration;
float distanceCm;
float distanceInch;
float distancem;

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
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
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
  distanceInch = distanceCm * CM_TO_INCH; //formula
  distancem = distanceCm/100; //formula
  
  //printing values
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  Serial.print("Distance in (m): ");
  Serial.println(distancem);
  Serial.println();
  //delay(200);


  ThingSpeak.writeField(CHANNEL_ID, 1 , distanceInch , CHANNEL_API_KEY);
  
  ThingSpeak.writeField(CHANNEL_ID, 2 , distancem , CHANNEL_API_KEY);
  
  ThingSpeak.writeField(CHANNEL_ID, 3 , distanceCm , CHANNEL_API_KEY);
  delay(1000);

  
}

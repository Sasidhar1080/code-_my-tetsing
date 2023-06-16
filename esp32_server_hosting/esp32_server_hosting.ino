#include<HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "time.h"

//#define MAIN_SSID "esw-m19@iiith"
//#define MAIN_PASS "e5W-eMai@3!20hOct"

#define MAIN_SSID "SCRC_LAB"
#define MAIN_PASS "Scrciiith@123"

#define CSE_IP      "192.168.0.178"
#define CSE_PORT    5683
#define HTTPS     false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "AE-TEST"
#define OM2M_DATA_CONT  "Node-1/Data"

const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 
long randNumber;

HTTPClient http;

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}


 // Values to store in server
long duration;
float distanceCm;
float distanceInch;
float distancem;

const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701


void setup() {
Serial.begin(115200);
configTime(0, 0, ntpServer);
WiFi.begin(MAIN_SSID,MAIN_PASS);

while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }

pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  randNumber = random(300);
  Serial.println(randNumber);
  
  //Sensor Block  DHT 
 static int i=0;
 float soil_moist=23;
// float temp=random(27,48);
// float rh=random(60,85);

digitalWrite(trigPin, LOW);
  delay(200);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delay(200);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  // Convert cm to m

  distancem = distanceCm/100;



 
String data="[" + String(distanceInch) + " , " + String(distancem)+ " , "+ String(distanceCm) + "]";

String server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;


http.begin(server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/");

http.addHeader("X-M2M-Origin", OM2M_ORGIN);
http.addHeader("Content-Type", "application/json;ty=4");
http.addHeader("Content-Length", "100");

String req_data = String() + "{\"m2m:cin\": {"

  + "\"con\": \"" + data + "\","

   + "\"rn\": \"" + "cin_"+String(i++) + "\","

  + "\"cnf\": \"text\""

  + "}}";
int code = http.POST(req_data);
http.end();
Serial.println(code);
delay(1000);
}

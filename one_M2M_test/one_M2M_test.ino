#include<HTTPClient.h>
#include <WiFi.h>
#include "time.h"

#define MAIN_SSID "SCRC_LAB"
#define MAIN_PASS "Scrciiith@123"

#define CSE_IP      "192.168.0.119"
#define CSE_PORT    2000
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

void setup() {
Serial.begin(115200);
configTime(0, 0, ntpServer);
WiFi.begin(MAIN_SSID,MAIN_PASS);

while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
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
 float temp=random(27,48);
 float rh=random(60,85);
 

 
String data="[" + String(temp) + " , " + String(rh)+ "]";

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

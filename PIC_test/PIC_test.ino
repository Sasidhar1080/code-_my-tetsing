#include <HTTPClient.h>
#include <WiFi.h>
#include <NTPClient.h>
//#include <WiFiUdp.h>

#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ) )
#define MAIN_SSID "sai"
#define MAIN_PASS "10.10.10.10"
#define CSE_IP      "dev-onem2m.iiit.ac.in"
#define CSE_PORT    443
#define HTTPS       true
#define OM2M_ORGIN    "devtest:devtest"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "AE-SR/SR-AQ"
#define OM2M_DATA_CONT  "SR-AQ-KH03-00/Data"

#include <NTPClient.h>
#include "time.h"

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime; 

// Function that gets current epoch time
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
// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(MAIN_SSID, MAIN_PASS);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

HTTPClient http;
int pin1= 5;
int pin2= 26;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  initWiFi();
  configTime(0, 0, ntpServer);
 }

void loop() {
  // put your main code here, to run repeatedly:
  int pir1=0, pir2=0;
  epochTime = getTime();
  Serial.println(epochTime);
  pir1 = digitalRead(pin1);
  delay(10);
  pir2 = digitalRead(pin2);
  Serial.print("pir1" + String(pir1));
  Serial.print("pir1" + String(pir1));

  
  String data="[" + String(epochTime)+ "," + String(pir1) + " , " + String(pir2)+ "]";
  Serial.print(data);
  String server;
  
  if(HTTPS == true){
   server="https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;
  }
  else{
    server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;
  }
  Serial.println(server);
  

  http.begin(server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/");
  Serial.println(server+ String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");
  
  String req_data = String() + "{\"m2m:cin\": {"
  
    + "\"con\": \"" + data + "\","
  
     + "\"rn\": \"" + "cin_"+String(epochTime) + "\","
  
    + "\"cnf\": \"text\""
  
    + "}}";
  int code = http.POST(req_data);
  http.end();
  Serial.println(code);
  delay(1000);
}

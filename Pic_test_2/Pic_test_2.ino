#include <HTTPClient.h>
#include <WiFi.h>
#include <NTPClient.h>
//#include <WiFiUdp.h>

#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ) )
#define MAIN_SSID "JioFi3_127A1C"
#define MAIN_PASS "0rn36u30sm"
#define CSE_IP      "192.168.225.64"
#define CSE_PORT    8080
#define HTTPS       false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "AE-Misc"
#define OM2M_DATA_CONT  "OC-KH95-02/Data"

#include <NTPClient.h>
#include "time.h"
int count1=0, count2=0;
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

//int pin1=15;
//int pin2=23;

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
  int occ1,occ2, switchoff1=0, thresh=60, switchoff2=0;
  int pir1=0, pir2=0;
  epochTime = getTime();
  Serial.println(epochTime);
  pir1 = digitalRead(pin1);
  delay(10);
  pir2 = digitalRead(pin2);
  Serial.print("pir1" + String(pir1));
  Serial.print("pir2" + String(pir2));
  //occ1 = pir1 && pir2;
  occ2 = pir1 || pir2;
  //Serial.print(occ1);
  Serial.print(occ2);
  //Serial.println("Count1"+ String(count1));
  Serial.print("Count2"+ String(count2));
  if (occ2==0)
  {
    count2+=1;
    if (count2>=thresh)
    {
      switchoff2 =1;
      count2=0;
    }
  }
  else{
    count2=0;
  }
  String data= "[" + String(epochTime)+ " , " + String(pir1) + " , " + String(pir2)+ " , " + String(switchoff2)+ "]";
  Serial.print(data);
  String server;
  
  if(HTTPS == true){
   server="https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;
  }
  else{
    server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;
  }
  Serial.println(server);
  

  http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");
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

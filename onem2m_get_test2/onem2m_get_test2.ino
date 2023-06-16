#include<HTTPClient.h>
#include <WiFi.h>
#include "time.h"

#define MAIN_SSID "SCRC_LAB"
#define MAIN_PASS "Scrciiith@123"

#define CSE_IP      "dev-onem2m.iiit.ac.in"
#define CSE_PORT    443
#define HTTPS     false
#define OM2M_ORGIN    "AirPoll@20:22uHt@Sas"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "AE-WM/WM-WD"
#define OM2M_DATA_CONT  "WM-WD-PL04-00/Data"

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

   String server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;

  http.begin(server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/");
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);

  int code = http.GET();
  if (code > 0) {
    String response = http.getString();
    Serial.print("HTTP response code: ");
    Serial.println(code);
    Serial.print("HTTP response body: ");
    Serial.println(response);
  } else {
    Serial.println("HTTP GET request failed");
  }
  http.end();

  delay(1000);

 
  // String server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;

  // http.begin(server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/");
  // http.addHeader("X-M2M-Origin", OM2M_ORGIN);

  // int code = http.GET();
  // http.end();
  // Serial.println(code);
  // delay(1000);
}

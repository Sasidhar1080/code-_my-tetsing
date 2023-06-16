#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed); 

// Example of data captured by IRrecvDumpV2.ino replace the array below with the numbers captured by IRrecvDumpV2.ino from the serial monitor 
uint16_t rawData[407] = {4976, 2186,  318, 1828,  318, 756,  318, 754,  318, 756,  342, 1804,  320, 752,  320, 754,  318, 756,  318, 756,  342, 1804,  320, 752,  320, 1830,  344, 1804,  344, 730,  320, 1830,  318, 1830,  342, 1804,  318, 1830,  342, 1806,  342, 730,  320, 1828,  342, 732,  342, 732,  342, 730,  344, 730,  320, 754,  342, 730,  342, 1804,  320, 752,  344, 730,  318, 1830,  318, 754,  320, 754,  344, 728,  320, 1830,  318, 756,  342, 732,  342, 730,  344, 730,  344, 730,  344, 730,  344, 730,  342, 730,  318, 754,  318, 754,  318, 756,  318, 756,  342, 730,  344, 730,  344, 1804,  320, 1828,  342, 1804,  320, 754,  320, 756,  342, 1806,  344, 728,  344, 29374,  5000, 2162,  320, 1828,  344, 728,  344, 730,  344, 730,  342, 1806,  342, 730,  318, 754,  344, 728,  320, 754,  344, 1804,  320, 752,  344, 1806,  318, 1828,  342, 732,  342, 1804,  320, 1828,  344, 1804,  318, 1828,  318, 1830,  342, 732,  342, 1804,  320, 754,  318, 754,  344, 730,  342, 730,  342, 730,  320, 754,  318, 1830,  342, 730,  342, 730,  344, 1806,  318, 754,  318, 756,  342, 730,  318, 754,  318, 754,  344, 730,  344, 730,  344, 730,  342, 730,  342, 1804,  320, 1830,  342, 730,  342, 730,  344, 1804,  344, 1806,  342, 1804,  318, 756,  318, 756,  342, 732,  342, 732,  342, 730,  344, 730,  342, 730,  318, 756,  342, 730,  342, 1804,  398, 676,  320, 756,  342, 730,  344, 730,  342, 1802,  320, 754,  318, 756,  342, 730,  342, 730,  342, 730,  344, 730,  344, 730,  344, 730,  342, 732,  342, 730,  342, 730,  342, 732,  318, 754,  342, 732,  342, 730,  342, 730,  344, 730,  342, 730,  344, 730,  342, 1804,  318, 754,  320, 1828,  344, 1804,  344, 730,  342, 736,  314, 754,  342, 732,  342, 730,  342, 730,  342, 730,  342, 1804,  318, 1830,  344, 730,  318, 754,  318, 754,  318, 756,  342, 732,  342, 730,  344, 730,  344, 730,  344, 730,  344, 730,  342, 730,  318, 754,  342, 730,  318, 754,  344, 732,  342, 1804,  320, 754,  344, 730,  344, 730,  342, 732,  342, 732,  342, 730,  342, 730,  342, 730,  320, 754,  344, 730,  342, 730,  344, 730,  344, 730,  344, 730,  344, 732,  342, 732,  342, 730,  342, 730,  344, 730,  344, 730,  344, 730,  320, 754,  342, 730,  318, 754,  344, 732,  342, 730,  342, 730,  342, 730,  344, 730,  344, 1804,  320, 754,  342, 732,  342, 1804,  320, 754,  320};  // UNKNOWN 13C03F81

int SR_OC_KH00_00;
int SR_OC_KH00_01;
int SR_OC_KH00_02;
int SR_OC_KH00_03;

int SR_OC[4];
int AC_STATE = 0;


int send_req();

WiFiClient client;
HTTPClient http;


#define MAIN_SSID "SCRC_LAB"
#define MAIN_PASS "Scrciiith@123"
// #define HTTPS     false
#define OM2M_ORGIN    "guest:guest"
bool connect_http();

void turnOnAc(){
  #if SEND_RAW
      irsend.sendRaw(rawData, 407, 38);  // Send a raw data capture at 38kHz.
  #endif  // SEND_RAW
}

void jsonParseOccupancy(String parsed_data){
  StaticJsonDocument<192> doc;

  DeserializationError error = deserializeJson(doc, parsed_data);
  
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  SR_OC_KH00_00 = int(doc["SR-OC-KH00-00"]); // "1"
  SR_OC[0] = SR_OC_KH00_00;
  SR_OC_KH00_01 = int(doc["SR-OC-KH00-01"]); // "1"
  SR_OC[1] = SR_OC_KH00_01;
  SR_OC_KH00_02 = int(doc["SR-OC-KH00-02"]); // "1"
  SR_OC[2] = SR_OC_KH00_02;
  SR_OC_KH00_03 = int(doc["SR-OC-KH00-03"]); // "1"
  SR_OC[3] = SR_OC_KH00_03;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    Serial.println("Initiating Network Connection.");
    WiFi.begin(MAIN_SSID,MAIN_PASS);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected successfully");
  irsend.begin();
}

void loop() {
  String URL= "http://onem2m.iiit.ac.in:443/~/in-cse/in-name/AE-SR/SR-OC/SR-OC-GW-KH00-00/Data/la";
  http.begin(client, URL);
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json");

  int status_code = http.GET();
  Serial.println(status_code);
  if(status_code == 200){
    int people_count = 0;
    String payload = http.getString();
//    Serial.println(payload);
  StaticJsonDocument<768> doc;

  DeserializationError error = deserializeJson(doc, payload);
  
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    String parsed_data;
    JsonObject m2m_cin = doc["m2m:cin"];
    const char* con = m2m_cin["con"];
//    Serial.println(con);
    for( int i = 13; i <= 100; i++){
      if(con[i] == '\''){
        parsed_data += "\"";
      } else{
        parsed_data += con[i];  
      }
    }
//    Serial.println(parsed_data);

    jsonParseOccupancy(parsed_data);
    Serial.println("OC Data: ");
    for(int i = 0; i < 4; i++){
      Serial.println(SR_OC[i]);
      if(SR_OC[i] == 0){
        people_count += 1;
      }
    }
      if(people_count >= 3){
    
    if(!AC_STATE){
      Serial.println("Turn AC ON!");
      // LOGIC TO ACTIVATE CIRCUIT
      turnOnAc();      
      Serial.println("Circuit Activated");  
      AC_STATE = 1;
      delay(2000);
    }
    
    
  } else if(people_count < 3){
    Serial.println("Turn AC OFF!");
    // LOGIC TO ACTIVATE CIRCUIT
    
  }
  }
  http.end();

  delay(2000);
}
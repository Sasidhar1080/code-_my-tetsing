#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <HTTPClient.h>
#define OLED_SDA 21
#define OLED_SCL 22
#include <time.h>
include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

Adafruit_SH1106 display(21, 22);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String formattedDate;
String dayStamp;
String timeStamp;
#include "Adafruit_SHT4x.h"
#include "Adafruit_SGP40.h"
#include "WiFi.h"
#include "ThingSpeak.h"

#define WIFI_NETWORK "SCRC_LAB"
#define WIFI_PASSWORD "Scrciiith@123"
#define OM2M_DATA_LBL  "[\"AE-SR-AQ\", \"SR-AQ-KH95-01\", \"V3.0.00\", \"SR-AQ-V3.0.00\"]"
const char* ntpServer = "pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime; 
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
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
// Suresh 1
unsigned long myChannelNumber = 1591608;
const char * myWriteAPIKey = "MGYZNCH5TAPELMYO";

//// Murali 2
//unsigned long myChannelNumber = 1843906;
//const char * myWriteAPIKey = "1SK357NZ84IDRES0";

//
//////Kiran 3
//unsigned long myChannelNumber = 1843903;
//const char * myWriteAPIKey = "EY0KSJFWLW7PDK3C";
//
////Vamshi
//unsigned long myChannelNumber = 1788132;
//const char * myWriteAPIKey = "WJ5QH39TFBNMAMFW";


unsigned int voc_index_G;
unsigned long duration, th, tl;
  int ppm;
float Temp_VOC;
float HUM_VOC;
int pm2;
int pm10;
int raw;
#define OM2M_Node_ID "SR-AQ-KH95-01"

//OM2M Components
#define CSE_IP "onem2m.iiit.ac.in"
#define HTTP   true
#define CSE_PORT 443
#define OM2M_ORGIN "AQSRMon@20:psX9MSnnrvyH"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "AE-SR/SR-AQ"
#define OM2M_DATA_CONT "/Data"

// HTTP Client Object
HTTPClient http;

// Server URL
String server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;
String path = server + String() + OM2M_AE + "/" + String()+OM2M_Node_ID + String()+OM2M_DATA_CONT + "/";

const unsigned char iiith [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xe0, 0x0c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xe7, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x81, 0x38, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfc, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xff, 0xff, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x2f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xef, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0xff, 0xff, 0x1f, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x8e, 0x7d, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xcf, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x8f, 0xff, 0xff, 0xff, 0xf1, 0xfe, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x8f, 0xff, 0xfd, 0xbf, 0xff, 0xfd, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x17, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xfd, 0xde, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xfe, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xcf, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x10, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xcf, 0xf8, 0xff, 0xfb, 0xfb, 0xff, 0xf0, 0x04, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe7, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xcf, 0xfc, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0xff, 0xff, 0x0f, 0xfe, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xf6, 0xff, 0xdf, 0xff, 0xc0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x77, 0xff, 0xff, 0xff, 0xf8, 0x7e, 0xf8, 0x3f, 0xff, 0xff, 0x7f, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x47, 0xff, 0xff, 0xff, 0xf8, 0xfe, 0xfc, 0x3f, 0xff, 0xff, 0xdf, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xdb, 0xff, 0xdf, 0xff, 0xf1, 0x3e, 0x3f, 0xff, 0xff, 0xff, 0x9b, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xff, 0xff, 0xbf, 0xff, 0xf0, 0x29, 0x3f, 0xff, 0xff, 0xff, 0xcd, 0x00, 0x00, 
0x00, 0x00, 0x05, 0xff, 0xff, 0xf3, 0xff, 0x84, 0x70, 0x07, 0xff, 0xff, 0xff, 0xc7, 0x80, 0x00, 
0x00, 0x00, 0x03, 0xff, 0xff, 0x83, 0xff, 0xc6, 0xe6, 0xc1, 0xff, 0xff, 0xff, 0xc7, 0x80, 0x00, 
0x00, 0x00, 0x07, 0xff, 0xfe, 0xc3, 0xff, 0xe6, 0xe7, 0xc1, 0xff, 0xf7, 0xff, 0xef, 0xe0, 0x00, 
0x00, 0x00, 0xff, 0xff, 0xfa, 0xc7, 0xff, 0xdf, 0x97, 0xc1, 0xff, 0xe7, 0xff, 0xf7, 0xa0, 0x00, 
0x00, 0x01, 0xcf, 0xff, 0x83, 0xc4, 0xff, 0xff, 0xe7, 0xcc, 0x7f, 0xfb, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x01, 0x8f, 0xff, 0x9f, 0xc5, 0xff, 0xff, 0xe5, 0xfe, 0x3f, 0xfb, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x00, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xef, 0xff, 0x3f, 0xef, 0xff, 0xff, 0x80, 0x00, 
0x00, 0x01, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xef, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 
0x00, 0x01, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xe5, 0x7f, 0xfe, 0x7f, 0xf8, 0x00, 0x00, 
0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xf3, 0x7f, 0xfc, 0x00, 0x00, 
0x00, 0x01, 0xff, 0xff, 0x7f, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xf8, 0x00, 0x00, 
0x00, 0x00, 0x7f, 0xe6, 0x7f, 0xd7, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0xa1, 0xec, 0x00, 0x00, 
0x00, 0x00, 0x07, 0x87, 0xff, 0x3f, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xc3, 0xc5, 0xed, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x06, 0x78, 0x3f, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xef, 0xcf, 0xf9, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xef, 0xff, 0xef, 0xff, 0x90, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x17, 0xfc, 0x37, 0xff, 0x3f, 0x7b, 0xff, 0xef, 0xbf, 0xc0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x13, 0xfc, 0x3f, 0xff, 0x3f, 0x4f, 0xff, 0xef, 0x00, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x12, 0x4c, 0x7e, 0x3f, 0xff, 0x59, 0xfc, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x12, 0x48, 0x3e, 0x3f, 0xfe, 0x19, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0x10, 0x48, 0x2e, 0x3f, 0xb0, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x0e, 0x1f, 0xf4, 0x01, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x08, 0x1f, 0x86, 0x00, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x1f, 0x82, 0x00, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x02, 0x3f, 0xc2, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3f, 0xe6, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x38, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
const unsigned char temp [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x87, 0xff, 0xbf, 0xf3, 0xc0, 0xf1, 0xfc, 0x01, 0x8f, 0xff, 0xbf, 0xf3, 0xc0, 0xf1, 0xff, 0x01, 
0x80, 0x70, 0x38, 0x03, 0xe1, 0xf1, 0xcf, 0x01, 0x80, 0x70, 0x38, 0x03, 0xe1, 0xf1, 0x87, 0x01, 
0x80, 0x70, 0x38, 0x03, 0xe3, 0xf1, 0x83, 0x81, 0x80, 0x70, 0x38, 0x03, 0xf3, 0xf1, 0x87, 0x01, 
0x80, 0x70, 0x3f, 0xe3, 0xb3, 0x71, 0x87, 0x01, 0x80, 0x70, 0x3f, 0xe3, 0xb7, 0x71, 0xff, 0x01, 
0x80, 0x70, 0x38, 0x03, 0xbf, 0x71, 0xfe, 0x01, 0x80, 0x70, 0x38, 0x03, 0x9e, 0x71, 0xc0, 0x01, 
0x80, 0x70, 0x38, 0x03, 0x9e, 0x71, 0x80, 0x01, 0x80, 0x70, 0x38, 0x03, 0x9e, 0x71, 0x80, 0x01, 
0x80, 0x70, 0x38, 0x03, 0x80, 0x71, 0x80, 0x01, 0x80, 0x70, 0x3f, 0xf3, 0x80, 0x71, 0x80, 0x01, 
0x80, 0x30, 0x3f, 0xf3, 0x80, 0x71, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xe7, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xe3, 0x80, 0x1c, 0x01, 0xf0, 0x01, 0x80, 0x01, 0xe3, 0x80, 0x3e, 0x07, 0xfc, 0x01, 
0x80, 0x01, 0xfb, 0x80, 0x77, 0x0f, 0xfc, 0x01, 0x80, 0x01, 0xfb, 0x80, 0x63, 0x1c, 0x0c, 0x01, 
0x80, 0x01, 0xfb, 0x80, 0x73, 0x1c, 0x00, 0x01, 0x80, 0x01, 0xfb, 0x80, 0x3e, 0x38, 0x00, 0x01, 
0x80, 0x01, 0xfb, 0x80, 0x1c, 0x38, 0x00, 0x01, 0x80, 0x01, 0xfb, 0x80, 0x00, 0x38, 0x00, 0x01, 
0x80, 0x01, 0xfb, 0xc0, 0x00, 0x38, 0x00, 0x01, 0x80, 0x03, 0xfb, 0xc0, 0x00, 0x38, 0x00, 0x01, 
0x80, 0x03, 0xdb, 0xe0, 0x00, 0x38, 0x00, 0x01, 0x80, 0x07, 0xbf, 0xe0, 0x00, 0x1c, 0x00, 0x01, 
0x80, 0x07, 0xfe, 0xe0, 0x00, 0x1e, 0x0c, 0x01, 0x80, 0x07, 0xfe, 0xe0, 0x00, 0x0f, 0xfc, 0x01, 
0x80, 0x07, 0xfe, 0xe0, 0x00, 0x07, 0xfc, 0x01, 0x80, 0x07, 0xff, 0xe0, 0x00, 0x01, 0xf0, 0x01, 
0x80, 0x07, 0xfd, 0xe0, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };



void ConnectToWifi() {
  Serial.println("Connecting To wifi!!");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  int start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected to Wifi");
  Serial.println(WiFi.localIP());
}

WiFiClient  client;

//------------------------------------------------CO2 Sensing-----------------------------------------------------
#define PIN 18


void CO2_Monitor() {
  
  th = pulseIn(PIN, HIGH, 2008000) / 1000;
  tl = 1004 - th;
  ppm = 2000 * (th - 2) / (th + tl - 4);

  if (ppm > 1000) {
    Serial.println(ppm);
    ThingSpeak.setField(5, ppm);
  }
  else {
    Serial.print(" Co2 Concentration: ");
    Serial.println(ppm);
    ThingSpeak.setField(5, ppm);
    //    Serial.println(" ppm");
  }
}

//--------------------------------------------------SHT4x---------------------------------------------------------------------------------------------------------------------------------------
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_SGP40 sgp;
void SHT_setup() {
  sht4.begin();
  sgp.begin();

}

void SHT_Reading() {

  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data


    Temp_VOC = temp.temperature;
    HUM_VOC = humidity.relative_humidity;

  Serial.print("Temperature: ");
  Serial.println(Temp_VOC);
  //  Serial.println(" degrees C");

  Serial.print("Humidity: ");
  Serial.println(HUM_VOC);
  //  Serial.println("% rH");

  ThingSpeak.setField(3, Temp_VOC);
  ThingSpeak.setField(4, HUM_VOC);
  
  raw = sgp.measureRaw(Temp_VOC, HUM_VOC);
  Serial.print("VOC: ");
  Serial.println(raw);
  ThingSpeak.setField(6, raw);
  voc_index_G = sgp.measureVocIndex(Temp_VOC, HUM_VOC);
  Serial.print("Voc Index: ");
  Serial.println(voc_index_G);
  ThingSpeak.setField(7,sgp.measureVocIndex(Temp_VOC, HUM_VOC) );
  delay(2000);
 
  delay(500);
}

//-------------------------------------------------SGP-------------------------------------------------------------------------------------------------------------------------------------------
//Adafruit_SGP40 sgp;
//
//void SGP_setup() {
//  Serial.println("SGP40 test");
//
//  if (! sgp.begin()) {
//    Serial.println("Sensor not found :(");
//  }
//}
//
//void SGP_Reading() {
//
//  int raw;
//  raw = sgp.measureRaw(temp.temperature, humidity.relative_humidity);
//  Serial.print("VOC: ");
//  Serial.println(raw);
//  ThingSpeak.setField(2, raw);
//  voc_index_G = sgp.measureVocIndex(Temp_VOC, HUM_VOC);
//  Serial.print("Voc Index: ");
//  Serial.println(voc_index_G);
//  delay(1000);
//}

//------------------------------------------------------Particulate Matter----------------------------------------------------------------------------------------------------------------------

unsigned long previous_loop, previous_10, previous_25, prev_time;

byte command_frame[9] = {0xAA, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x67, 0xBB};
byte received_data[9];
int sum = 0;
void send_command(byte command)
{
  command_frame[1] = command;
  int sum = command_frame[0] + command_frame[1] + command_frame[2] + command_frame[3] + command_frame[4] + command_frame[5] + command_frame[8];
  int rem = sum % 256;
  command_frame[6] = (sum - rem) / 256;
  command_frame[7] = rem;
  delay(1000);
  Serial.write(command_frame, 9);

}

bool checksum()
{
  sum = int(received_data[0]) + int(received_data[1]) + int(received_data[2]) + int(received_data[3]) + int(received_data[4]) + int(received_data[5]) + int(received_data[8]);
  if (sum == ((int(received_data[6]) * 256) + int(received_data[7])))
  {
    return true;
  }
  else
    return false;
}
void calculate_pm()
{
  pm2 = int(received_data[4]) * 256 + int(received_data[5]);
  delay(2000);
  pm10 = int(received_data[2]) * 256 + int(received_data[3]);
  ThingSpeak.setField(1, pm2);
  ThingSpeak.setField(2, pm10);
  Serial.print("PM2.5: ");
  Serial.println(pm2);
  Serial.print("PM10: ");
  Serial.println(pm10);
}

void PM_setup() {
  send_command(0x01);
}

void PM_Reading() {
  delay(5000);
  if (millis() - prev_time > 5000)
  {
    send_command(0x02);
    prev_time = millis();
  }
  if (Serial.available())
  {
    Serial.readBytes(received_data, 9);
    if (checksum())
    {
      calculate_pm();
    }
  }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  initWiFi();
  configTime(0, 0, ntpServer);
  pinMode(OLED_SDA, INPUT_PULLUP);           // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(500);
  //Serial.begin(9600);
  //  delay(5000);
  ConnectToWifi();
  ThingSpeak.begin(client);
  SHT_setup();
  //  SGP_setup();
  PM_setup();
//  display_setup();
timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);
}

int counter = 0;
  void loop() {
    while(!timeClient.update()) {
      timeClient.forceUpdate();
    }
    // The formattedDate comes with the following format:
    // 2018-05-28T16:00:13Z
    // We need to extract date and time
    formattedDate = timeClient.getFormattedDate();
    Serial.println(formattedDate);
  
    // Extract date
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    Serial.print("DATE: ");
    Serial.println(dayStamp);
    // Extract time
    timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);
  CO2_Monitor();
  SHT_Reading();
  // SGP_Reading();
  PM_Reading();
 // display_loop();
epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  delay(1000);
  int prev_time = millis();

  while ((WiFi.status() != WL_CONNECTED) && ((millis() - prev_time) < 5000))  {
    // WiFi.mode(WIFI_STA);
    Serial.println("Reconnecting to WiFi...");
    WiFi.reconnect();
  }

// Display Text
delay(5000);
   display.clearDisplay();
  display.setTextSize(2                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     );
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.println("pm_2.5 =");
  display.println(pm2);
  display.display();
  delay(5000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.println("pm_10 =");
  display.println(pm10);
  display.display();
  delay(5000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,8); 
  display.println("Voc index");
  display.println(voc_index_G);
  
  display.display();
  delay(5000);
   display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.println("temp =");
  display.println(Temp_VOC);
  display.display();
  delay(5000);
   display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.println("rH =");
  display.println(HUM_VOC);
  display.display();
  delay(5000);
   display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.println("CO2 = ");
  display.println(ppm);
  display.display();
  delay(5000);
    
  display.clearDisplay();

 
  //  if (WiFi.status() == WL_CONNECTED) {
  //    Serial.println("Connected ");
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(2000);

http.begin(path);
  Serial.println(path);

  // Required Headers
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
  //float occupancy=random(0,2);
  //float distance=random(0,400);
  // Actual Data
  String data = "[" + String(epochTime) + ", " + String(pm2) + ", " + String(pm10) + " , " + String(ppm) + ", " + String(voc_index_G) + ", " + String(raw) + " , " + String(Temp_VOC) + ", " + String(HUM_VOC) + "]";
  Serial.println(data);
  //http.begin(server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  // Request Body with data
  String req_data = String() + "{\"m2m:cin\": {"

    +"\"con\": \"" + data + "\","

    +    "\"lbl\": " + OM2M_DATA_LBL + ","

    +"\"cnf\": \"text\""

    +"}}";
  int code = http.POST(req_data);
  http.end();
  if (code == 201) {
    Serial.println("Data Posted Successfully");
  } else {
    Serial.println("Data posting failed with http code-" + String(code));
  }  

  //  Serial.println("---------------------------\n\n");
}

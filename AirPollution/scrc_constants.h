/*
   scrc_constant.h

    Created on: 12-May-2022
        Author: Ayu Parmar
*/

#ifndef INCLUDE_SCRC_CONSTANT_H_
#define INCLUDE_SCRC_CONSTANT_H_

/*
   uncomment to print debug statements
*/
#define DEBUG false
#define ONEM2M false

/*
   node type
*/

#define NT_POLLUTION
//#define NT_WATER

/*
   Controller type
*/
#define CT_ESP_8266
//#define CT_ESP_32

/*
   uncomment to publish to om2m
*/
#define PUB_OM2M

/*
   uncomment to publish to ThingSpeak
*/
#define PUB_THINGSPEAK

/*
   time interval in milliseconds
*/
#define TIME_INTERVAL_15_SEC  15000

#define TIME_INTERVAL_1_HR    3600000             //1 * 60* 60 * 1000

#define TIME_INTERVAL_10_MIN  600000             //10*60 * 1000
#define TIME_INTERVAL_1_MIN  60000             //10*60 * 1000

// count of sensor reading in 10 min @ 15 sec, i.e., 4 reading per min, (40)
#define PRIMARY_BUF_COUNT  40

// count of 10 min avg sensor reading in 1 days, i.e., 6 reading per 1 hour, (144 reading)
#define SEC_BUF_COUNT  100

/*
   WiFi credentials
*/
/*#define MAIN_SSID "JioFi3_B51AF6"
  #define MAIN_PASS "8kj617hspt"
*/
#define MAIN_SSID "CHD"
#define MAIN_PASS "F326@36968237"

#define RETRY_WIFI_INTERVAL 60000  // in ms (1 min)
#define RETRY_WIFI_FACTOR 2     //multiplication factor user to increment the retry interval

#define STARTUP_WIFI_TIMEOUT  60000   //WiFi connection Timeout
#define STARTUP_WIFI_RETRY_DELAY 100  // WiFi reconnection delay

#define Timeoffset 19800

#define Reading_10min 2

#define MAX_STRING_LEN 255

/*
   OneM2M connection details
*/
#define CSE_IP      "onem2m.iiit.ac.in"
#define CSE_PORT    443
#define HTTPS     true
#define FINGER_PRINT  "10 3D D5 4E B1 47 DB 4B 5C B0 89 08 41 A7 A4 14 87 10 7F E8"
#define OM2M_ORGIN    "AirPoll@20:22uHt@Sas"
#define OM2M_MN       "/~/in-cse/in-name/"
#define OM2M_AE       "AE-AQ"

#define OM2M_DATA_CONT  "AQ-AN00-00/Data"
#define OM2M_DATA_LBL   "[\"AE-AQ\", \"AQ-AN00-00\", \"V3.0.02\", \"AQ-V3.0.02\"]"

/*
  Debug Function
*/
#if DEBUG
#define debug_info(fmt, ...) Serial.print(fmt, ##__VA_ARGS__)
#define debug_err(fmt, ...) Serial.print(fmt, ##__VA_ARGS__)
#else
#define debug_info(fmt, ...) ((void)0)
#define debug_err(fmt, ...) ((void)0)
#endif

/*
   Error Handling Codes
*/
#define E_OM2M_NW -101
#define E_OM2M_CONNECT -102
#define E_OM2M_CONNECTION -103
#define E_OM2M_NO_RESPONSE -104
#define E_OM2M_EMPTY_RESPONSE -105

#define E_THINGSPEAK_NW -201
#define E_THINGSPEAK_CONNECT -202
#define E_THINGSPEAK_CONNECTION -203
#define E_THINGSPEAK_NO_RESPONSE -204
#define E_THINGSPEAK_EMPTY_RESPONSE -205

/*
   Success Codes
*/
#define SUCCESS_OM2M 400
#define SUCCESS_THINGSPEAK 800

#endif /* INCLUDE_SCRC_CONSTANT_H_ */

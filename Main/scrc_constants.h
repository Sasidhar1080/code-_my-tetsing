/*
 * scrc_constants.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#ifndef SCRC_CONSTANTS_H_
#define SCRC_CONSTANTS_H_

/*
 uncomment to print debug statements
 */
#define DEBUG true

/*
 Node Number
 */
#define NO_OF_NODES 1
/*
 node type
 */
/*
 * HTTP
 */
#define HTTP true

//#define NT_POLLUTION
#define NT_WATER

/*
 Controller type
 */
//#define CT_ESP_8266
#define CT_ESP_32
/*
 * Project Information
 */

/*
 * Water Monitoring
 */
#define WM_WD_RO
//#define WM_WD_OH
//#define WM_WD_OH_ALL
//#define WM_WF

#define flag_level  false
/*
 Sensor PIN Definitions
 */
#define myArgs(...) __VA_ARGS__

#define INIT_ARR(VAR_NAME,ARR_DATA) int VAR_NAME[] = {myArgs ARR_DATA}

#if defined(WM_WD_RO)|| defined(WM_WD_OH_ALL)
#define MOTOR_OUT_PIN 16
#define MOTOR_IN_PIN 17
#endif

#if defined(WM_WD_RO) || defined(WM_WD_OH_ALL)
#define TDS_SENSOR_PIN 35
#define TEMP_SENSOR_PIN 27

#endif

#if defined(WM_WD_OH) || defined(WM_WD_OH_ALL)
#define Turbidity_SENSOR_PIN 34
#define pH_SENSOR_PIN 14
#endif

#if defined(WM_WD_OH) ||defined(WM_WD_OH_ALL) || defined(WM_WD_RO)

#define ULTRASONIC_SENSOR_TRIGGER_PIN 19
#define ULTRASONIC_SENSOR_ECHO_PIN 18

#endif

/*OLED Address
*/
#define OLED_ADDR 0x3C
#define I2C_SDA 21
#define I2C_SCL 22
#define DISPLAY_BUF_COUNT 10
/*
 uncomment to publish to om2m
 */

#define PUB_OM2M

/*
 uncomment to publish to ThingSpeak
 */
#define PUB_THINGSPEAK

#define PRESSURE_MODBUS
/*
 time interval in milliseconds
 */
//#define TIME_INTERVAL_5_MIN  300000L //5L * 60L * 1000L
//#if false

/*
 Time interval for WM_WD_RO is 4 hrs
 */
#ifdef WM_WD_RO
#define TIME_INTERVAL_PRIMARY  60000L
#define TIME_INTERVAL_OM2M  60000L
#define TIME_INTERVAL_THINGSPEAK  60000L
/*
 Time interval for WM_WD_OH is 1hr
 */
/*
 Time interval for WM_WD_OH_ALL is 1hr
 */

#elif defined(WM_WD_OH) || defined(WM_WD_OH__ALL)
	#define TIME_INTERVAL_PRIMARY 60000L
	#define TIME_INTERVAL_OM2M 60000L
	#define TIME_INTERVAL_THINGSPEAK 60000L
/*
 Time interval for WM_WD_OH is 1 minute
 */

#elif defined(WM_WF)
	#define TIME_INTERVAL_PRIMARY 60000L
	#define TIME_INTERVAL_OM2M 60000L
	#define TIME_INTERVAL_THINGSPEAK 60000L


#else
	#define TIME_INTERVAL_PRIMARY 60000L
	#define TIME_INTERVAL_OM2M 60000L
	#define TIME_INTERVAL_THINGSPEAK 60000L


#endif
// count of sensor reading in 1500 min @ 5 min, i.e., 1 reading per 5 min
#define PRIMARY_BUF_COUNT  300

/*
 WiFi credentials
 */
#if HTTP
//#define MAIN_SSID "JioFi3_259D0E"
//#define MAIN_PASS "k61m12m0u5"
//#define MAIN_SSID "SCRC_LAB"
//#define MAIN_PASS "Scrciiith@123"
//#define MAIN_SSID "TP-Link_3934"
//#define MAIN_PASS "wdsdtumt9r"
#define MAIN_SSID "esw-m19@iiith"
#define MAIN_PASS "e5W-eMai@3!20hOct"


#else

//#define MAIN_SSID "JioFi3_259D0E"
//#define MAIN_PASS "k61m12m0u5"
#define MAIN_SSID "esw-m19@iiith"
#define MAIN_PASS "e5W-eMai@3!20hOct"
//#define MAIN_SSID "SCRC_LAB"
//#define MAIN_PASS "Scrciiith@123"
#endif

#define RETRY_WIFI_INTERVAL 60000  // in ms (1 min)
#define RETRY_WIFI_FACTOR 2     //multiplication factor user to increment the retry interval

#define STARTUP_WIFI_TIMEOUT  60000   //WiFi connection Timeout
#define STARTUP_WIFI_RETRY_DELAY 100  // WiFi reconnection delay
#define WIFI_TIMEOUT 30000
#define Timeoffset 19800
#define MAX_STRING_LEN 255

/*
 OneM2M connection details
 */
#define CSE_IP      "onem2m.iiit.ac.in"
//#define CSE_IP      "dev-onem2m.iiit.ac.in"
#define CSE_PORT    443

#define OM2M_ORGIN    "wdmon@20:gpod@llk4"
//#define OM2M_ORGIN    "devtest:devtest"
#define OM2M_MN     "/~/in-cse/in-name/AE-WM/"

#define OM2M_AE     "WM-WD"
#define OM2M_DATA_CONT  "WM-WD-VN01-01/Data"

#ifdef WM_WD_RO
#define NODE_ID "WM-WD-VN01-01"
#define NODE_DS_VER "V2.0.0"

#define OM2M_DATA_LBL_0 "[\"AE-WM-WD\", \""+ NODE_ID + "\", \"" + NODE_DS_VER + "\", \" WM-WD-" + NODE_DS_VER + "\"]"
#define OM2M_DATA_CONT_0 NODE_ID +"/Data"

#elif defined(WM_WD_OH)

#define NODE_ID "WM-WD-VN00"
#define NODE_DS_VER "V2.0.0"

#define OM2M_DATA_LBL_0 "[\"AE-WM-WD\", \""+ NODE_ID + "\", \"" + NODE_DS_VER + "\", \" WM-WD-" + NODE_DS_VER + "\"]"
#define OM2M_DATA_CONT_0 NODE_ID +"/Data"

#elif defined(WM_WD_OH_ALL)

#define NODE_ID "WM-WD-KN00-00"
#define NODE_DS_VER "V3.0.0"

#define OM2M_DATA_LBL_0 "[\"AE-WM-WD\", \""+ NODE_ID + "\", \"" + NODE_DS_VER + "\", \" WM-WD-" + NODE_DS_VER + "\"]"
#define OM2M_DATA_CONT_0 NODE_ID +"/Data"

#elif defined(WM_WF)

#define no_nodes 4

#define NODE_ID_0 "WM-WD-VN00"
#define NODE_ID_1 "WM-WD-VN01"
#define NODE_ID_2 "WM-WD-VN02"
#define NODE_ID_3 "WM-WD-VN03"

#define NODE_DS_VER "V4.0.0"

#define OM2M_DATA_LBL_0 "[\"AE-WM-WD\", \""+ NODE_ID_0 + "\", \"" + NODE_DS_VER + "\", \" WM-WD-" + NODE_DS_VER + "\"]"
#define OM2M_DATA_CONT_0 NODE_ID_0 +"/Data"

#define OM2M_DATA_LBL_1 "[\"AE-WM-WD\", \""+ NODE_ID_1 + "\", \"" + NODE_DS_VER + "\", \" WM-WD-" + NODE_DS_VER + "\"]"
#define OM2M_DATA_CONT_1 NODE_ID_1 +"/Data"

#define OM2M_DATA_LBL_2 "[\"AE-WM-WD\", \""+ NODE_ID_2 + "\", \"" + NODE_DS_VER + "\", \" WM-WD-" + NODE_DS_VER + "\"]"
#define OM2M_DATA_CONT_2 NODE_ID_2 +"/Data"
#define OM2M_DATA_LBL_3 "[\"AE-WM-WD\", \""+ NODE_ID_3 + "\", \"" + NODE_DS_VER + "\", \" WM-WD-" + NODE_DS_VER + "\"]"
#define OM2M_DATA_CONT_3 NODE_ID_3 +"/Data"

#endif

/*
 ThingSpeak connection details
 */
#define THING_SPEAK_ADDRESS "api.thingspeak.com"
//#define WRITEAPIKEY_0  "XJKXA878EBFAQEUA"; //VN00-00

//#define WRITEAPIKEY_0  "SVT68G6UN42KW8IL"; //VN01-00

#define WRITEAPIKEY_0  "IL9NQXBGYYDKQMDE"; //VN01-01

//#define WRITEAPIKEY_0  "OQXJ7222GJO59EQI"; //VN90-00

//#define WRITEAPIKEY_0  "Y2RONI5EFI06DET1"; //PH01-00

//#define WRITEAPIKEY_0  "XBIKUKHJ7QIYFX4M"; //AD98-00

//#define WRITEAPIKEY_0  "PFN9TIHWYN6H2615"; //PH02-00

//#define WRITEAPIKEY_0  "7T8ESH7QODTQBP6X"; //PL00-00

//#define WRITEAPIKEY_0  "TKG2SLEQFGKNA8UM"; //PH03-00

//#define WRITEAPIKEY_0  "RD0LMQIB63EOPNOU"; //KN00-00

/* #if defined(WM_WF)
#define WRITEAPIKEY_1  "AKG7JNRL1YRYDHLL"; //VN1000
#define WRITEAPIKEY_2  "AKG7JNRL1YRYDHLL"; //VN1000
#define WRITEAPIKEY_3  "AKG7JNRL1YRYDHLL"; //VN1000
#endif */

#define TIMEOUT_NETWORK  5000

#if defined(WM_WD_RO)
#define NUM_FIELDS 5
#elif defined(WM_WD_OH_ALL)
#define NUM_FIELDS 8
#elif defined(WM_WD_OH)
#define NUM_FIELDS 4
#elif defined(WM_WF)
#define NUM_FIELDS 5
#endif

#if defined(WM_WD_RO) || defined(WM_WD_OH_ALL)
/*
 Motor Values
*/
#define MOTOR_OUT_OFF LOW
#define MOTOR_IN_OFF LOW
#define MOTOR_OUT_ON HIGH
#define MOTOR_IN_ON HIGH
#define MOTOR_INPUT_ON_INTERVAL 3000L //10 seconds
#define MOTOR_OUTPUT_ON_INTERVAL 5000L//10 seconds
#define MOTOR_OUT_BUFFER_TIME 1000L
#define TIME_INTERVAL_CLEAN  5*60*60*1000L

/*
 Container Dimensions
 */
#define CONTAINER_HEIGHT 6
#define BUFFER_HEIGHT 1
#define CLEAN_HEIGHT 1

#endif


/*
 * WaterFlow-ModBus
 */
#if defined(WM_WF)
#define WF_ADDRESS_1 0x01
#define WF_ADDRESS_2 0x02
#define WF_ADDRESS_3 0x03
#define WF_ADDRESS_4 0x04
#define WF_UART_DATA_BIT                  8
#define WF_UART_PARITY                    NONE
#define WF_UART_STOP_BIT                  1

#define WF_UART_TX                        27
#define WF_UART_RX                        26
#define WF_UART_RTS                       25
#define WF_UART_BAUD_RATE                 9600
#define WF_UART_CONFIG                    SERIAL_8N1

//Water Flow Defines
#define WF_POLLING_INTERVAL            10000
//#define WF_ADDRESS                     0x01

#define WF_REG_ADDR                    0
#define WF_REG_LEN                     20

#define WF_FLOW_RATE                    0
#define WF_TOTAL_FLOW                   6

/*
 * PressureSensor-ModBus
 */

#define PRESSURE_PIN_1 35
#define PRESSURE_PIN_2 36
#define PRESSURE_PIN_3 34
#define PRESSURE_PIN_4 32

#define PS_UART_DATA_BIT                  8
#define PS_UART_PARITY                    NONE
#define PS_UART_STOP_BIT                  1

#define PS_UART_TX                        27
#define PS_UART_RX                        26
#define PS_UART_RTS                       25
#define PS_UART_BAUD_RATE                 9600
#define PS_UART_CONFIG                    SERIAL_8N1

//Pressure Sensor Defines
#define PS_POLLING_INTERVAL            10000
#define PS_ADDRESS                     0x03
#define PS_REG_ADDR                    0
#define PS_REG_LEN                     19

#define PS_Value                       17
#endif

/*
 Debug Function
 */
#if DEBUG
#define debug_info(fmt, ...) Serial.println(fmt, ##__VA_ARGS__)
#define debug_err(fmt, ...) Serial.println(fmt, ##__VA_ARGS__)
#else
#define debug_info(fmt, ...) ((void)0)
#define debug_err(fmt, ...) ((void)0)
#endif

/*
 * Error Handling Codes
 */

#define E_DSB_SEARCH -1000
#define E_DSB_CRC -1001
#define E_DSB_DEVICE -1002

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
 * Success Codes
 */
#define SUCCESS_OM2M 400
#define SUCCESS_THINGSPEAK 800
#endif /* SCRC_CONSTANTS_H_ */

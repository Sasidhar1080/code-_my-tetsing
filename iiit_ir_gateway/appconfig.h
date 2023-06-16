#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__             1
    
    #include <stdint.h>
	#include "driver/i2c.h"
	#include "driver/spi_master.h"
	#include "driver/uart.h"

    /* Define these macros to run test cases */
//	#define RUN_WIFI_MQTT_TEST					1
//	#define RUN_RTC_TEST						1
//	#define RUN_SNTP_TEST						1
//	#define RUN_SPIFS_TEST						1
//	#define RUN_SDCARD_FS_TEST					1

    /* Disable watchdog timer functionality */
//    #define APP_DISABLE_WDT                   1

	#define CLI_UART							UART_NUM_1
	#define CLI_UART_BAUDRATE					115200
	#define CLI_UART_BUFFSIZE					512

	#define NRF_UART							UART_NUM_2
	#define NRF_UART_BAUDRATE					115200
	#define NRF_UART_BUFFSIZE					256

    /* I2C0 Port used for communication */
    #define BOARD_I2C                 			I2C_NUM_0
	#define BOARD_I2C_FREQ					  	4*100*1000

	/* Retry connecting to the AP after these many seconds. */
	#define WIFIAGENT_RETRY_DELAY				10

	/* MQTT Agent config. */
//	#define BROKER_URL 							"mqtts://a1q49k1dxjqytg-ats.iot.ap-south-1.amazonaws.com"
//	#define BROKER_URL							"mqtts://mqtt.eclipse.org:8883"
	#define MQTT_RX_QUEUE_SIZE					5
	#define MQTT_TX_QUEUE_SIZE					5
	#define MQTT_PUBACK_TIMEOUT					10

	#define APP_WIFI_SSID_DEF					"myssid"
	#define APP_WIFI_PASS_DEF					"password"
	#define APP_BROKER_URL_DEF					"mqtt://mqtt.eclipse.org"
	#define APP_MQTT_PORT_DEF					1883

	/* App logic task configuration. */
	/* Maximum number of MAC IDs to be stored in FLASH. */
	#define MAX_MOTION_SNSR_MACIDS				10
	/* NRF pulse event will be polled these many no of times. */
	#define NRF_CONFIG_TIMEOUT					20
	/* Motion sensor status will be polled every this interval (multiple of 0.5 secs) */
	/* Reducing this will help sending the LED states frequently. */
	#define NRF_POLL_INTERVAL					4
	/* Ir off cmd will be sent every this interval (multiple of 0.5 secs) */
	#define IR_OFF_INTERVAL						100 /*anu342023*/
	/* Ir train pins will be low for this amount of time (units ms). */
	#define IR_TRAIN_DELAY						2000
	/* Ir off cmd pulse generation delays (units ms). */
	#define IR_CMD_PULSE_DELAY					100
	#define IR_CMD_SILENT_DELAY					1000

	/* Define this to bypass control of reset pins of
	 * NRF and MAXQ6. This is required to program the chipsets
	 * using an external programmer.
	 * Remember to disable this setting after you program the
	 * firmware in NRF and MAXQ otherwise this firmware
	 * will not function as per the spec. */
//	#define APP_MAKE_RST_HIZ					1

	/* App tasks priority and stack size. */
	#define PRIORITY_WIFI						(tskIDLE_PRIORITY + 5)
	#define STACK_WIFI							4096
	#define PRIORITY_MQTT						(tskIDLE_PRIORITY + 5)
	#define STACK_MQTT							4096
	#define PRIORITY_APP						(tskIDLE_PRIORITY + 5)
	#define STACK_APP							4096
	#define PRIORITY_UART_CLI					(tskIDLE_PRIORITY + 5)
	#define STACK_UART_CLI						4096

	#define APP_NVS_NAMESPACE					"appnvs"
	#define APP_NVS_KEY							"appkey"

	/* Rec upload states */
    typedef enum uploadStates
    {
       UPLOAD_IDLE, UPLOAD_INIT, UPLOAD_WAITING
    }uploadStates;

	/* Switch states */
    typedef enum 
    {
       SW_RELEASED, SW_DETECTING, SW_DEBOUNCING, SW_PRESSED
    }sw_states_t;

    typedef struct
    {
    	uint8_t ssid[32];
    	uint8_t passwd[64];
    	uint8_t staticIp[20];
    	uint8_t subnetMsk[20];
    	uint8_t gateway[20];
    	uint8_t brokerUrl[100];
    	int32_t port;
    	uint8_t macIdTable[MAX_MOTION_SNSR_MACIDS][6];
    	uint8_t macTableLen;
    }nvs_const_t;

#ifdef APP_MAKE_RST_HIZ
	#warning "NRF and MAXQ Reset is HIGHZ."
#endif

#endif

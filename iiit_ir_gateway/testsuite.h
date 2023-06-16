/*
 * testsuite.h
 *
 * Created: 04-Jul-18 4:39:15 PM
 *  Author: Mahesh
 */ 


#ifndef __TESTSUITE_H__
#define __TESTSUITE_H__

#include "appconfig.h"
#include "boards.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Wifi + MQTT test. */
#ifdef RUN_WIFI_MQTT_TEST
	#include "wifiagent.h"
	#include "mqttagent.h"
	void WifiMqttTest(void);
#endif

/* RTC Driver Test */
#ifdef RUN_RTC_TEST
    #include "mcp7940m.h"
    void RTCTest(uint8_t setTime);
#endif

#ifdef RUN_SNTP_TEST
#endif

#ifdef RUN_SPIFS_TEST
	#include "extspifs_api.h"
	void SPIFSTest(void);
#endif


#ifdef RUN_EEPROM_TEST
#endif

#endif /* TESTSUITE_H_ */

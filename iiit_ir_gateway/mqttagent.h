/*
 * mqttagent.h
 *
 *  Created on: Jan 13, 2020
 *      Author: Mahesh
 */

#ifndef __MQTTAGENT_H__
#define __MQTTAGENT_H__				1

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "boards.h"
#include "appconfig.h"
#ifdef MQTT_AWS_MODE
	/* Include certificate header files. */
	#include "awscerts.h"
#endif

#define MQTT_CONNECT_BIT			BIT0
#define MQTT_DISCONNECT_BIT			BIT1
#define MQTT_SUBACK_BIT				BIT2
#define MQTT_PUBACK_BIT				BIT3
#define MQTT_ERROR_BIT				BIT4

typedef struct
{
	uint8_t *dataPtr;
	int dataLen;
}mqttDataPkt_t;

void vTaskMQTT(void *pvArg);
EventBits_t WaitMQTTConnect(TickType_t xTicksToWait);
EventBits_t WaitMQTTDisConnect(TickType_t xTicksToWait);
BaseType_t MQTTWriteToPubQ(mqttDataPkt_t *dataPktPtr, TickType_t xTicksToWait);
BaseType_t MQTTReadFromSubQ(mqttDataPkt_t **dataPktPtr, TickType_t xTicksToWait);
uint8_t MQTTIsSubQEmpty(void);

#endif /* __MQTTAGENT_H__ */

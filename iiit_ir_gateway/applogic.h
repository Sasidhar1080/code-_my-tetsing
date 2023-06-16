/*
 * applogic.h
 *
 *  Created on: Mar 11, 2020
 *      Author: Mahesh
 */

#ifndef __APPLOGIC_H__
#define __APPLOGIC_H__

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "mqttagent.h"
#include "uartcli.h"
#include "sht21.h"
#include "esp_log.h"
#include "boards.h"
#include "appconfig.h"

/* Allowed MQTT commands. */
/* Issue off command pulse to IR. */
#define MQTT_CMD_IROFF			1
/* Register MAC IDs to scan. */
#define MQTT_CMD_REGMAC			2
/* Read MAC IDs to scan. */
#define MQTT_CMD_READMAC		3
/* Read current occupancy status and RH&T */
#define MQTT_CMD_READSTATUS		4

#define IR_CMD_ID0				BIT0
#define IR_CMD_ID1				BIT1
#define IR_CMD_ID2				BIT2

void vTaskAppLogic(void *pvArg);

void SetWifiLedState(uint8_t state);
void SetMqttLedState(uint8_t state);

#endif /* __APPLOGIC_H__ */

/*
 * pppd.h
 *
 *  Created on: Jan 13, 2020
 *      Author: Mahesh
 */

#ifndef __WIFIAGENT_H__
#define __WIFIAGENT_H__			1

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "boards.h"
#include "appconfig.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

#define WIFIEVT_CONNECT_BIT				BIT0
#define WIFIEVT_DISCONNECT_BIT			BIT1
#define WIFIEVT_APPDISCONNECT_BIT		BIT2
#define WIFIEVT_STA_STARTED_BIT			BIT3

#define NVSVAR_SAVE_SSID				BIT0
#define NVSVAR_SAVE_PASS				BIT1
#define NVSVAR_SAVE_STATICIP			BIT2
#define NVSVAR_SAVE_BROKERURL			BIT3
#define NVSVAR_SAVE_PORT				BIT4
#define NVSVAR_SAVE_MACTABLE			BIT5
#define NVSVAR_SAVE_ALL					0x0000FFFF

void InitWifiAgent(void);
void SetDefaultConfig(nvs_const_t *var);
void GetCurrentConfig(nvs_const_t *var);
void SaveConfig(nvs_const_t *var, uint32_t flags);

void vTaskWifi(void *pvArg);
EventBits_t WaitWifiConnect(TickType_t xTicksToWait);
EventBits_t WaitWifiAppDisConnect(TickType_t xTicksToWait);

#endif /* __WIFIAGENT_H__ */

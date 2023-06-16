/*
 * pppd.c
 *
 *  Created on: Jan 13, 2020
 *      Author: Mahesh
 */

#include "wifiagent.h"
#include <string.h>
#include "applogic.h"

typedef enum
{
	WIFI_STATE_CONNECTWAIT, WIFI_STATE_CONNECTED, WIFI_STATE_DISCONNECTED
}wifiStates_t;

static const char *TAG = "WIFI-AGENT";
static EventGroupHandle_t wifiEventGrp = NULL;
nvs_const_t nvsVar = {};

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        xEventGroupSetBits(wifiEventGrp, WIFIEVT_STA_STARTED_BIT);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
    	xEventGroupClearBits(wifiEventGrp, WIFIEVT_CONNECT_BIT);
    	xEventGroupSetBits(wifiEventGrp, WIFIEVT_DISCONNECT_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->ip_info.ip));
        xEventGroupSetBits(wifiEventGrp, WIFIEVT_CONNECT_BIT);
        xEventGroupClearBits(wifiEventGrp, WIFIEVT_DISCONNECT_BIT | WIFIEVT_APPDISCONNECT_BIT);
    }
}

static void ReadNvsVars(void)
{
	nvs_handle_t handle;
	esp_err_t err;
	size_t blobSize = sizeof(nvs_const_t);

	err = nvs_open(APP_NVS_NAMESPACE, NVS_READWRITE, &handle);
	if (err != ESP_OK)
	{
		/* Unable to read stored config. Load default config. */
		SetDefaultConfig(&nvsVar);
		return;
	}
	err = nvs_get_blob(handle, APP_NVS_KEY, &nvsVar, &blobSize);
	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
	{
		/* Unable to read stored config. Load default config. */
		SetDefaultConfig(&nvsVar);
	}
	nvs_close(handle);
}

void SetDefaultConfig(nvs_const_t *var)
{
	strcpy((char*)var->ssid, APP_WIFI_SSID_DEF);
	strcpy((char*)var->passwd, APP_WIFI_PASS_DEF);
	strcpy((char*)var->brokerUrl, APP_BROKER_URL_DEF);
	strcpy((char*)var->staticIp, "0.0.0.0");
	var->port = APP_MQTT_PORT_DEF;
	var->macTableLen = 0;
}

void GetCurrentConfig(nvs_const_t *var)
{
	memcpy(var, &nvsVar, sizeof(nvs_const_t));
}

void SaveConfig(nvs_const_t *var, uint32_t flags)
{
	nvs_handle_t handle;
	esp_err_t err;
	size_t blobSize = sizeof(nvs_const_t);

	/* Update values only which are indicated in the flags. */
	if(flags & NVSVAR_SAVE_SSID)
	{
		strcpy((char*)nvsVar.ssid, (char*)var->ssid);
	}

	if(flags & NVSVAR_SAVE_PASS)
	{
		strcpy((char*)nvsVar.passwd, (char*)var->passwd);
	}

	if(flags & NVSVAR_SAVE_BROKERURL)
	{
		strcpy((char*)nvsVar.brokerUrl, (char*)var->brokerUrl);
	}

	if(flags & NVSVAR_SAVE_PORT)
	{
		nvsVar.port = var->port;
	}

	if(flags & NVSVAR_SAVE_STATICIP)
	{
		strcpy((char*)nvsVar.staticIp, (char*)var->staticIp);
		strcpy((char*)nvsVar.subnetMsk, (char*)var->subnetMsk);
		strcpy((char*)nvsVar.gateway, (char*)var->gateway);
	}

	if(flags & NVSVAR_SAVE_MACTABLE)
	{
		memcpy(nvsVar.macIdTable, var->macIdTable, sizeof(nvsVar.macIdTable));
		nvsVar.macTableLen = var->macTableLen;
	}

	err = nvs_open(APP_NVS_NAMESPACE, NVS_READWRITE, &handle);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Config Save Error 1.");
		return;
	}
	err = nvs_set_blob(handle, APP_NVS_KEY, &nvsVar, blobSize);
	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
	{
		ESP_LOGE(TAG, "Config Save Error 2.");
	}
	nvs_close(handle);
}

void InitWifiAgent(void)
{
	wifiEventGrp = xEventGroupCreate();
	/* Indicate to the application that WiFi is disconnected. */
	xEventGroupSetBits(wifiEventGrp, WIFIEVT_APPDISCONNECT_BIT);

	/* Initialize NVS before calling the WiFi Apis. */
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
	  ESP_ERROR_CHECK(nvs_flash_erase());
	  ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	/* Read user configuration from NVS. */
	ReadNvsVars();

	tcpip_adapter_init();

	ESP_ERROR_CHECK(esp_event_loop_create_default());

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

	/* Set SSID & Passwd here if not using NVS vals. */
	wifi_config_t wifi_config = {};

	strcpy((char*)wifi_config.sta.ssid, (char*)nvsVar.ssid);
	strcpy((char*)wifi_config.sta.password, (char*)nvsVar.passwd);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());
}

void vTaskWifi(void *pvArg)
{
	wifiStates_t state = WIFI_STATE_CONNECTWAIT;
	uint32_t connStatus = 0;

	/* Wait for the station mode initialized event. */
	xEventGroupWaitBits(wifiEventGrp, WIFIEVT_STA_STARTED_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
	esp_wifi_connect();
	SetWifiLedState(0);

	while(1)
	{
		switch(state)
		{
			case WIFI_STATE_CONNECTWAIT:
				/* Waiting for Wifi connect event. */
				ESP_LOGI(TAG, "Connect Wait.");
				connStatus = xEventGroupWaitBits(wifiEventGrp, WIFIEVT_CONNECT_BIT |
															   WIFIEVT_DISCONNECT_BIT,
															   pdFALSE, pdFALSE, portMAX_DELAY);
				if(connStatus & WIFIEVT_CONNECT_BIT)
				{
					state = WIFI_STATE_CONNECTED;
				}
				else if(connStatus & WIFIEVT_DISCONNECT_BIT)
				{
					state = WIFI_STATE_DISCONNECTED;
				}
				break;
			case WIFI_STATE_CONNECTED:
				ESP_LOGI(TAG, "Connected");
				SetWifiLedState(1);
				/* If connected to AP, don't do anything, just wait. */
				connStatus = xEventGroupWaitBits(wifiEventGrp, WIFIEVT_DISCONNECT_BIT,
															   pdTRUE, pdTRUE, portMAX_DELAY);
				/* If got disconnected, retry connecting to the AP. */
				if(connStatus & WIFIEVT_DISCONNECT_BIT)
				{
					state = WIFI_STATE_DISCONNECTED;
					SetWifiLedState(0);
				}
				break;
			case WIFI_STATE_DISCONNECTED:
				/* Notify application that WiFi is disconnected. */
				xEventGroupClearBits(wifiEventGrp, WIFIEVT_CONNECT_BIT);
				xEventGroupSetBits(wifiEventGrp, WIFIEVT_APPDISCONNECT_BIT);
				ESP_LOGI(TAG, "Dis-Connected, Silent Wait.");
				vTaskDelay(pdMS_TO_TICKS(WIFIAGENT_RETRY_DELAY*1000));
				ESP_LOGI(TAG, "Retry connect.");
				esp_wifi_connect();
				state = WIFI_STATE_CONNECTWAIT;
				break;
		}
	}
}

EventBits_t WaitWifiConnect(TickType_t xTicksToWait)
{
	if(wifiEventGrp!=NULL)
	{
		return xEventGroupWaitBits(wifiEventGrp, WIFIEVT_CONNECT_BIT, pdFALSE, pdTRUE, xTicksToWait);
	}
	return 0;
}

EventBits_t WaitWifiAppDisConnect(TickType_t xTicksToWait)
{
	if(wifiEventGrp!=NULL)
	{
		return xEventGroupWaitBits(wifiEventGrp, WIFIEVT_APPDISCONNECT_BIT, pdFALSE, pdTRUE, xTicksToWait);
	}
	return 0;
}

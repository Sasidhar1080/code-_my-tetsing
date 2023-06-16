/*
 * mqttagent.c
 *
 *  Created on: Jan 13, 2020
 *      Author: Mahesh
 */

#include "wifiagent.h"
#include "mqttagent.h"
#include "applogic.h"

typedef enum
{
	MQTT_WAIT_WIFI_CONN, MQTT_WAIT_HOST_CONN, MQTT_TRY_SUB,
	MQTT_WAIT_SUB, MQTT_CONNECTED, MQTT_WAIT_PUBACK,
	MQTT_SEND_STORE, MQTT_DISCON
}mqttStates_t;

static const char *TAG = "MQTTAgent";
static esp_mqtt_client_config_t mqtt_config;
static esp_mqtt_client_handle_t clientHandle;
char macString[16];
static char clientId[20];
static char topicData[30];
static char topicRx[30], topicTx[30];
static EventGroupHandle_t mqttEventGrp = NULL;
static QueueHandle_t mqttRxQHandle, mqttTxQHandle;

extern nvs_const_t nvsVar;

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    mqttDataPkt_t *dataPktPtr = NULL;

    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        xEventGroupClearBits(mqttEventGrp, MQTT_DISCONNECT_BIT);
		xEventGroupSetBits(mqttEventGrp, MQTT_CONNECT_BIT);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        xEventGroupClearBits(mqttEventGrp, MQTT_CONNECT_BIT);
		xEventGroupSetBits(mqttEventGrp, MQTT_DISCONNECT_BIT);
        break;
    case MQTT_EVENT_SUBSCRIBED:
    	ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    	xEventGroupSetBits(mqttEventGrp, MQTT_SUBACK_BIT);
    	//msg_id = esp_mqtt_client_publish(client, "/topic/esp-pppos", "esp32-pppos", 0, 0, 0);
        //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        xEventGroupSetBits(mqttEventGrp, MQTT_PUBACK_BIT);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
//        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
//        printf("DATA=%.*s\r\n", event->data_len, event->data);
//        xEventGroupSetBits(event_group, GOT_DATA_BIT);
        /* Check if control queue has empty slots. */
        if(uxQueueSpacesAvailable(mqttRxQHandle) > 0)
        {
        	/* Allocate 1 data pkt. */
			dataPktPtr = calloc(1, sizeof(mqttDataPkt_t));
			dataPktPtr->dataPtr = calloc(event->data_len, sizeof(char));
			memcpy(dataPktPtr->dataPtr, event->data, event->data_len);
			dataPktPtr->dataLen = event->data_len;
        	/* Write to control rx queue. */
			xQueueSend(mqttRxQHandle, &dataPktPtr, 0);
        }
		else
		{
			ESP_LOGI(TAG, "MQTT_EVENT_DATA: Queue Full.");
		}
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        xEventGroupSetBits(mqttEventGrp, MQTT_ERROR_BIT);
        break;
    default:
        ESP_LOGI(TAG, "MQTT other event id: %d", event->event_id);
        break;
    }
    return ESP_OK;
}

void vTaskMQTT(void *pvArg)
{
	uint32_t connStatus = 0;
	uint8_t i = 0, subRetry = 0;
	mqttDataPkt_t *dataPktPtr = NULL;

	SetMqttLedState(0);

	/* Read MAC ID from the efuse (should be unique). */
	uint64_t id = 0;
	esp_read_mac((uint8_t *)&id, ESP_MAC_WIFI_STA);

	mqttStates_t state = MQTT_WAIT_WIFI_CONN;
	mqttEventGrp = xEventGroupCreate();
	xEventGroupSetBits(mqttEventGrp, MQTT_DISCONNECT_BIT);
	mqtt_config.event_handle = mqtt_event_handler;
	/* Assuming that this is printing a 32 bit integer without 0 padding */
	sprintf(macString, "%X%X", (uint32_t)(id>>32), (uint32_t)id);
	sprintf(clientId, "ESP-%s", macString);
	sprintf(topicTx, "%s/Tx", macString);
	sprintf(topicRx, "%s/Rx", macString);
	mqtt_config.client_id = clientId;

	/* Certificates are required if you use TLS / AWS. */
	#ifdef MQTT_AWS_MODE
		mqtt_config.port = 8883;
		mqtt_config.cert_pem = AWS_CACERT;
	//const char cert[] = MQTT_ECLIPSE_CACERT;
	//mqtt_config.cert_pem = cert;
		mqtt_config.client_cert_pem = AWS_CLIENT_CERT;
		mqtt_config.client_key_pem = AWS_CLIENT_KEY;
	#endif

	/* MQTT Topics */
	ESP_LOGI(TAG, "Client ID %s", clientId);
	ESP_LOGI(TAG, "Topic Data %s", topicData);
	ESP_LOGI(TAG, "Topic Ctrl Rx %s", topicRx);
	ESP_LOGI(TAG, "Topic Ctrl Tx %s", topicTx);

	/* Init MQTT Rx & Tx queues. */
	mqttRxQHandle = xQueueCreate(MQTT_RX_QUEUE_SIZE, sizeof(mqttDataPkt_t *));
	assert(mqttRxQHandle!=NULL);
	mqttTxQHandle = xQueueCreate(MQTT_TX_QUEUE_SIZE, sizeof(mqttDataPkt_t *));
	assert(mqttTxQHandle!=NULL);

	while(1)
	{
		switch(state)
		{
			case MQTT_WAIT_WIFI_CONN:
				ESP_LOGI(TAG, "WAIT WiFi Connect");
				connStatus = WaitWifiConnect(portMAX_DELAY);
				/* Check if WiFi connected */
				if(connStatus & WIFIEVT_CONNECT_BIT)
				{
					xEventGroupClearBits(mqttEventGrp, MQTT_ERROR_BIT);
					/* Set broker configuration. */
					mqtt_config.uri = (char*)nvsVar.brokerUrl;
					mqtt_config.port = nvsVar.port;
					clientHandle = esp_mqtt_client_init(&mqtt_config);
					esp_mqtt_client_start(clientHandle);
					state = MQTT_WAIT_HOST_CONN;
					i = 0;
					subRetry = 0;
				}
				break;
			case MQTT_WAIT_HOST_CONN:
				connStatus = WaitMQTTConnect(pdMS_TO_TICKS(1000));
				if(connStatus & MQTT_CONNECT_BIT)
				{
					ESP_LOGI(TAG, "HOST CON");
					state = MQTT_TRY_SUB;
				}
				else
				{
					i++;
					if(i>=30)
					{
						/* More than 30 seconds have passed,
						 * may be WiFi connected but no internet /
						 * or broker not available.
						 * */
						state = MQTT_DISCON;
					}
				}
				break;
			case MQTT_TRY_SUB:
				/* Subscribe to all the topics. */
				if(esp_mqtt_client_subscribe(clientHandle, topicRx, 0)!=-1)
				{
					ESP_LOGI(TAG, "WAIT SUB");
					state = MQTT_WAIT_SUB;
				}
				else
				{
					/* If connection lost. */
					connStatus = WaitMQTTConnect(0);
					if(!(connStatus & MQTT_CONNECT_BIT))
					{
						state = MQTT_DISCON;
					}
					else
					{
						/* Count retries and disconnect if count crosses threshold. */
						if(subRetry)
						{
							state = MQTT_DISCON;
						}
						else
						{
							subRetry = 1;
						}
					}
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				break;
			case MQTT_WAIT_SUB:
				connStatus = xEventGroupWaitBits(mqttEventGrp, MQTT_SUBACK_BIT, pdTRUE, pdTRUE, pdMS_TO_TICKS(5000));
				if(connStatus & MQTT_SUBACK_BIT)
				{
					ESP_LOGI(TAG, "SUBACK OK");
					ESP_LOGI(TAG, "CONNECTED");
					/* Subscribe successful. */
					state = MQTT_CONNECTED;
					SetMqttLedState(1);
				}
				else
				{
					/* Subscribe un-successful. Disconnect and retry. */
					state = MQTT_DISCON;
				}
				break;
			case MQTT_CONNECTED:
				connStatus = xEventGroupWaitBits(mqttEventGrp, MQTT_DISCONNECT_BIT | MQTT_ERROR_BIT, pdFALSE, pdFALSE, 0);
				if((connStatus & MQTT_DISCONNECT_BIT) || (connStatus & MQTT_ERROR_BIT))
				{
					/* Client got disconnected or some other error occourred. */
					state = MQTT_SEND_STORE;
					SetMqttLedState(0);
				}
				/* Check if any cmd is to be sent on published topics. */
				if(pdTRUE == xQueueReceive(mqttTxQHandle, &dataPktPtr, pdMS_TO_TICKS(1000)))
				{
					/* Send contents on mqtt topic. */
					if(dataPktPtr!=NULL)
					{
						/* Control messages are published with 0 qos. */
						esp_mqtt_client_publish(clientHandle, topicTx, (char*)dataPktPtr->dataPtr, dataPktPtr->dataLen, 0, 0);
						ESP_LOGI(TAG, "PUB CTRL");
						/* Free memory */
						free(dataPktPtr->dataPtr);
						free(dataPktPtr);
						dataPktPtr = NULL;
					}
				}
				break;
			case MQTT_WAIT_PUBACK:
				ESP_LOGI(TAG, "WAIT PUB ACK");
				connStatus = xEventGroupWaitBits(mqttEventGrp, MQTT_PUBACK_BIT, pdTRUE, pdFALSE, pdMS_TO_TICKS(MQTT_PUBACK_TIMEOUT));
				/* If successful ack is not received. */
				if(!(connStatus & MQTT_PUBACK_BIT))
				{
					/* Timeout occurred and ack not received. Send data pkt to store in memory. */
				}
				else
				{
					ESP_LOGI(TAG, "PUB ACK DATA");
				}
				state = MQTT_CONNECTED;
				break;
			case MQTT_SEND_STORE:
				/* Client got disconnected. Clear the tx Queue. */
				while(uxQueueMessagesWaiting(mqttTxQHandle) > 0)
				{
					/* Read from tx queue and free the memory. */
					xQueueReceive(mqttTxQHandle, &dataPktPtr, 0);
					/* Free memory */
					if(dataPktPtr!=NULL)
					{
						free(dataPktPtr->dataPtr);
						free(dataPktPtr);
						dataPktPtr = NULL;
					}
				}
				state = MQTT_DISCON;
				break;
			case MQTT_DISCON:
				esp_mqtt_client_destroy(clientHandle);
				clientHandle = 0;
				state = MQTT_WAIT_WIFI_CONN;
				break;
		}
	}
}

EventBits_t WaitMQTTConnect(TickType_t xTicksToWait)
{
	if(mqttEventGrp!=NULL)
	{
		return xEventGroupWaitBits(mqttEventGrp, MQTT_CONNECT_BIT, pdFALSE, pdTRUE, xTicksToWait);
	}
	return 0;
}

EventBits_t WaitMQTTDisConnect(TickType_t xTicksToWait)
{
	if(mqttEventGrp!=NULL)
	{
		return xEventGroupWaitBits(mqttEventGrp, MQTT_DISCONNECT_BIT, pdFALSE, pdTRUE, xTicksToWait);
	}
	return 0;
}

BaseType_t MQTTWriteToPubQ(mqttDataPkt_t *dataPktPtr, TickType_t xTicksToWait)
{
	/* Write to publish queue. */
	if(mqttTxQHandle != NULL)
	{
		return xQueueSend(mqttTxQHandle, &dataPktPtr, xTicksToWait);
	}
	return pdFALSE;
}

BaseType_t MQTTReadFromSubQ(mqttDataPkt_t **dataPktPtr, TickType_t xTicksToWait)
{
	/* Read from sub queue. */
	if(mqttRxQHandle != NULL)
	{
		return xQueueReceive(mqttRxQHandle, dataPktPtr, xTicksToWait);
	}
	return pdFALSE;
}

uint8_t MQTTIsSubQEmpty(void)
{
	/* Write to publish queue. */
	if(mqttRxQHandle != NULL)
	{
		return (uxQueueMessagesWaiting(mqttRxQHandle) == 0);
	}
	return 1;
}

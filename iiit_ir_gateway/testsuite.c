/*
 * testsuite.c
 *
 * Created: 03-Sep-18 1:20:32 PM
 *  Author: Mahesh
 */ 

#include "testsuite.h"

#ifdef RUN_WIFI_MQTT_TEST
	#include "wifiagent.h"
	#include "mqttagent.h"
	void WifiMqttTest(void)
	{
    	uint32_t connStatus = 0;
    	const char *TAG = "WIFIMQTT_TEST";
    	char testData[20];
    	uint8_t i = 0;
    	uint16_t len = 0;
    	mqttDataPkt_t *dataPktPtr = NULL;

    	while(1)
    	{
    		connStatus = WaitMQTTConnect(pdMS_TO_TICKS(10000));
    		if(connStatus & MQTT_CONNECT_BIT)
    		{
    			ESP_LOGI(TAG, "MQTT Connected.");
    			break;
    		}
    		else
    		{
    			ESP_LOGI(TAG, "Waiting for MQTT Connect.");
    		}
    	}

    	while(1)
		{
    		ESP_LOGI(TAG, "Publish Pkt %d", ++i);
    		sprintf(testData, "Test Data %d", i);
    		len = strlen(testData);

    		dataPktPtr = calloc(1, sizeof(mqttDataPkt_t));
			dataPktPtr->dataPtr = calloc(len, sizeof(char));
			memcpy(dataPktPtr->dataPtr, testData, len);
			dataPktPtr->dataLen = len;

			MQTTWriteToPubQ(dataPktPtr, portMAX_DELAY);

			vTaskDelay(pdMS_TO_TICKS(10000));
		}
	}
#endif

/* RTC Driver Test */
#ifdef RUN_RTC_TEST
	void RTCTest(uint8_t setTime)
	{
		uint8_t buff[10];

		if(setTime)
		{
			buff[0] = 0x35;
			buff[1] = 0x13;
			buff[2] = 0x12;
			MCP79SetTime(buff);
			/* Enable battery backup so that time is retained even when power fails */
			MCP79BackupEn();
		}

		/* Set time compulsorily while you transact with the RTC for
		the first time to start the oscillator. */

		while(true)
		{
			MCP79GetTime(buff);
			printf("HH = %2X MM = %2X SS = %2X\n", buff[2], buff[1], buff[0] );
			fflush(stdout);
			vTaskDelay(1000/portTICK_PERIOD_MS);
		}
	}
#endif

#ifdef RUN_SPIFS_TEST
	#include "extspifs_api.h"
	static const char *TAG = "SPIFFS_TEST";

	void SPIFSTest(void)
	{
		spiffs_file fp = 0;
		ext_spiffs_t efs;
		char buff[20];
		int8_t bytes = 0;
		int i = 1;

		vTaskDelay(pdMS_TO_TICKS(10000));
		assert(extspifs_init(&efs)==ESP_OK);

		fp = SPIFFS_open(&efs.fs, "hello.txt", SPIFFS_O_APPEND | SPIFFS_O_CREAT | SPIFFS_O_WRONLY, 0);
		sprintf(buff,"Hello\r\n");
		SPIFFS_write(&efs.fs, fp, buff, strlen(buff));
		SPIFFS_close(&efs.fs, fp);

		vTaskDelay(pdMS_TO_TICKS(1000));
		ESP_LOGI(TAG, "Read Output:");

		fp = SPIFFS_open(&efs.fs, "hello.txt", SPIFFS_O_RDONLY, 0);

		while(1)
		{
			memset(buff, 0, 1);
			bytes = SPIFFS_read(&efs.fs, fp, buff, 1);
			if(bytes<=0)
			{
				break;
			}
			else
			{
				if(buff[0] == '\r')
				{
					printf("%d", i++);
				}
				printf("%c", buff[0]);
			}
		}
		SPIFFS_close(&efs.fs, fp);
		fflush(0);
		while(1)
		{
			vTaskDelay(portMAX_DELAY);
		}

	}
#endif

/* Web Server Test */
#ifdef RUN_WEB_SERVER_TEST
    const char htmlPg[] = "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <title>Document</title></head><body> <h1> Hello World </h1> <script></script></body></html>";
    //static httpd_handle_t server = NULL;
    void WebServerTest(void)
	{

	}
#endif

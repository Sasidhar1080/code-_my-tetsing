/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "wifiagent.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "boards.h"
#include "appconfig.h"
#include "testsuite.h"
#include "mqttagent.h"
#include "applogic.h"
#include "uartcli.h"

void app_main()
{
    /* Run Wifi Agent Task. */
	InitWifiAgent();
	assert(xTaskCreate(vTaskWifi, "WIFI", STACK_WIFI, NULL, PRIORITY_WIFI, NULL) == pdTRUE);
	/* Run MQTT Agent Task. */
	assert(xTaskCreate(vTaskMQTT, "MQTT", STACK_MQTT, NULL, PRIORITY_MQTT, NULL) == pdTRUE);

	#ifdef RUN_WIFI_MQTT_TEST
		WifiMqttTest();
	#endif

	/* Run App Logic Task. */
	assert(xTaskCreate(vTaskAppLogic, "APPL", STACK_APP, NULL, PRIORITY_APP, NULL) == pdTRUE);
	/* Run UART CLI Task. */
	assert(xTaskCreate(vTaskUartCli, "UART-CLI", STACK_UART_CLI, NULL, PRIORITY_UART_CLI, NULL) == pdTRUE);

}

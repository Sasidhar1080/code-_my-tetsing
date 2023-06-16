/*
 * cli.h
 *
 *  Created on: Mar 11, 2020
 *      Author: Mahesh
 */

#ifndef __UART_CLI_H__
#define __UART_CLI_H__				1

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "boards.h"
#include "appconfig.h"
#include "mqttagent.h"
#include "wifiagent.h"

/* Valid cmds for UART CLI. */
#define CLI_CMD_SSID				"ssid"
#define CLI_CMD_PASSWD				"pass"
#define CLI_CMD_STATIC_IP			"static-ip"
#define CLI_CMD_BROKER_URL			"broker"
#define CLI_CMD_BROKER_PORT			"port"
#define CLI_CMD_RESTORE_DEF			"default"
#define CLI_CMD_SAVE_CONF			"save"
#define CLI_CMD_DISCARD_CONF		"discard"
#define CLI_CMD_PRINT_CONF			"print"
#define CLI_CMD_TRAIN_IR			"train-ir"
#define CLI_CMD_MAC					"mac"
#define CLI_CMD_HELP				"help"
#define CLI_CMD_RESET				"devrst"
#define CLI_CMD_RESP_OK				"ok\r\n"
#define CLI_CMD_RESP_FAIL			"fail\r\n"
#define CLI_CMD_RESP_INVALID		"invalid cmd try help\r\n"
#define CLI_CMD_RESP_DEVRST			"the device will now reset...\r\n"
#define CLI_MSG_RST_DONE			"reset done.\r\n"

void vTaskUartCli(void *pvArg);

#endif /* __UART_CLI_H__ */

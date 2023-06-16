/*
 * cli.c
 *
 *  Created on: Mar 11, 2020
 *      Author: Mahesh
 */

#include "uartcli.h"
#include <string.h>

uint8_t uartRxBuff[CLI_UART_BUFFSIZE];
uint8_t uartTxBuff[CLI_UART_BUFFSIZE];
nvs_const_t tempNvsVar = {};
uint32_t saveFlags = 0;
uint8_t resetEsp = 0;

extern char macString[16];
char helpStr[] = "\r\n"
				 "Following commands are supported.\r\n"
				 CLI_CMD_SSID " <ssid of WiFi Ap>\r\n"
				 CLI_CMD_PASSWD " <your WiFi password>\r\n"
				 CLI_CMD_STATIC_IP " <ip for static / 0.0.0.0 for DHCP>\r\n"
				 CLI_CMD_BROKER_URL " <url / ip>\r\n"
				 CLI_CMD_BROKER_PORT " < broker mqtt port >\r\n"
				 CLI_CMD_TRAIN_IR " < index of ir channel to train >\r\n"
				 CLI_CMD_MAC " - will display MAC ID of this device.\r\n"
				 CLI_CMD_RESTORE_DEF " - will reset all the config to default vals.\r\n"
				 CLI_CMD_DISCARD_CONF " - will discard any unsaved config changes.\r\n"
				 CLI_CMD_SAVE_CONF " - will save the current config to NVM.\r\n"
				 CLI_CMD_PRINT_CONF " - will print the current config to console.\r\n"
				 "\r\n";

static char * CliCmdGetParamPtr(char *cmdStr, uint8_t paramIndex, uint8_t *paramLen)
{
	uint16_t cmdLen = strlen(cmdStr);
	uint8_t paramCount = 0;
	uint16_t charIndex = 0;
	char *paramPtr = NULL;
	/* Skip the cmd string until first space. */
	for(charIndex = 0; charIndex < cmdLen; charIndex++)
	{
		if(cmdStr[charIndex] == ' ')
			break;
	}
	if(charIndex != cmdLen)
	{
		do
		{
			/* Skip spaces. */
			while(cmdStr[charIndex] == ' ')
				charIndex++;
			if(charIndex != cmdLen)
			{
				paramCount++;
				if(paramIndex == paramCount)
				{
					/* Found parameter index, store it to return. */
					paramPtr = &cmdStr[charIndex];
					*paramLen = 0;
					/* Loop till next space is found. */
					while((cmdStr[charIndex] != ' ') && (cmdStr[charIndex] != '\0'))
					{
						charIndex++;
						/* calculate parameter length. */
						(*paramLen)++;
					}
					break;
				}
				else
				{
					/* Loop till next space is found. */
					while((cmdStr[charIndex] != ' ') && (cmdStr[charIndex] != '\0'))
					{
						charIndex++;
					}
				}
			}
			else
				break;
		}while(charIndex != cmdLen);
	}

	return paramPtr;
}

static void ExecuteCliCmd(uint8_t * inpBuff, uint8_t *outBuff, uint16_t *outLen)
{
	char *cmdStart = NULL;
	char *paramPtr = NULL;
	uint8_t paramLen = 0;
	uint32_t ip = 0;
	*outLen = 0;
	/* 0 = No Resp, 1 = OK Resp, 2 = Fail Resp, 3 = Invalid Resp. */
	uint8_t genResp = 0;
	uint8_t trainIr = 0;

	/* Extract command. */
	if((cmdStart = strstr((char*)inpBuff, CLI_CMD_SSID)) != NULL)
	{
		paramPtr = CliCmdGetParamPtr(cmdStart, 1, &paramLen);
		if((paramPtr!= NULL) && (paramLen != 0))
		{
			/* Copy ssid to temp var. */
			memcpy(tempNvsVar.ssid, paramPtr, paramLen);
			tempNvsVar.ssid[paramLen] = '\0';
			/* Set update ssid flag. */
			saveFlags |= NVSVAR_SAVE_SSID;
			/* Send OK response. */
			genResp = 1;
		}
		else
		{
			/* Send Failed response. */
			genResp = 2;
		}
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_PASSWD)) != NULL)
	{
		paramPtr = CliCmdGetParamPtr(cmdStart, 1, &paramLen);
		if((paramPtr!= NULL) && (paramLen != 0))
		{
			/* Copy password to temp var. */
			memcpy(tempNvsVar.passwd, paramPtr, paramLen);
			tempNvsVar.passwd[paramLen] = '\0';
			/* Set update password flag. */
			saveFlags |= NVSVAR_SAVE_PASS;
			/* Send OK response. */
			genResp = 1;
		}
		else
		{
			/* Send Failed response. */
			genResp = 2;
		}
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_STATIC_IP)) != NULL)
	{
		paramPtr = CliCmdGetParamPtr(cmdStart, 1, &paramLen);
		if((paramPtr!= NULL) && (paramLen != 0))
		{
			/* Parse IP address. */
			if(paramLen == 7)
			{
				if(memcmp(paramPtr, "0.0.0.0", paramLen) == 0)
				{
					/* Select DHCP. */
					strcpy((char*)tempNvsVar.staticIp, "0.0.0.0");
					/* Send OK response. */
					genResp = 1;
					/* Set update static-ip flag. */
					saveFlags |= NVSVAR_SAVE_STATICIP;
				}
			}
			else
			{
				/* First count no of . in received string.
				 * Then do other stuff.
				 * */
				ip = ipaddr_addr(paramPtr);
				if(ip != 0xFFFFFFFF)
				{
					/* Select static ip. */
					memcpy(tempNvsVar.staticIp, paramPtr, paramLen);
					/* Terminate the string. */
					tempNvsVar.staticIp[paramLen] = '\0';
					/* Send OK response. */
					genResp = 1;
					/* Set update static-ip flag. */
					saveFlags |= NVSVAR_SAVE_STATICIP;
				}
				else
				{
					/* Send fail response. */
					genResp = 2;
				}
			}
		}
		else
		{
			/* Send Failed response. */
			genResp = 2;
		}
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_BROKER_URL)) != NULL)
	{
		paramPtr = CliCmdGetParamPtr(cmdStart, 1, &paramLen);
		if((paramPtr!= NULL) && (paramLen != 0))
		{
			/* Copy broker url to temp var. */
			memcpy(tempNvsVar.brokerUrl, paramPtr, paramLen);
			tempNvsVar.brokerUrl[paramLen] = '\0';
			/* Set update broker url flag. */
			saveFlags |= NVSVAR_SAVE_BROKERURL;
			/* Send OK response. */
			genResp = 1;
		}
		else
		{
			/* Send Failed response. */
			genResp = 2;
		}
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_BROKER_PORT)) != NULL)
	{
		paramPtr = CliCmdGetParamPtr(cmdStart, 1, &paramLen);
		if((paramPtr!= NULL) && (paramLen != 0))
		{
			tempNvsVar.port = strtol(paramPtr, NULL, 10);
			if((tempNvsVar.port > 0) && (tempNvsVar.port <= 65535))
			{
				/* Set update port flag. */
				saveFlags |= NVSVAR_SAVE_PORT;
				/* Send OK response. */
				genResp = 1;
			}
			else
			{
				/* Send Failed response. */
				genResp = 2;
			}
		}
		else
		{
			/* Send Failed response. */
			genResp = 2;
		}
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_RESTORE_DEF)) != NULL)
	{
		/* Init default vals to temp var. */
		SetDefaultConfig(&tempNvsVar);
		saveFlags |= NVSVAR_SAVE_ALL;
		genResp = 1;
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_SAVE_CONF)) != NULL)
	{
		/* Write temp var vals to flash. */
		SaveConfig(&tempNvsVar, saveFlags);
		saveFlags = 0;
		genResp = 1;
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_DISCARD_CONF)) != NULL)
	{
		saveFlags = 0;
		/* Re-Init temp var from NVS var. */
		GetCurrentConfig(&tempNvsVar);
		genResp = 1;
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_PRINT_CONF)) != NULL)
	{
		/* Print existing configuration. */
		sprintf((char*)outBuff, "\r\n"
						 "SSID: %s\r\n"
						 "PASS: %s\r\n"
						 "STATIC-IP: %s\r\n"
						 "BROKER: %s\r\n"
						 "BROKER-PORT: %d\r\n"
						 "MAC: %s\r\n\r\n",
						 tempNvsVar.ssid,
						 tempNvsVar.passwd,
						 (strcmp((char*)tempNvsVar.staticIp, "0.0.0.0") == 0) ? "DHCP": (char*)tempNvsVar.staticIp,
						 tempNvsVar.brokerUrl,
						 tempNvsVar.port,
						 macString);
		*outLen = strlen((char*)outBuff);
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_TRAIN_IR)) != NULL)
	{
		paramPtr = CliCmdGetParamPtr(cmdStart, 1, &paramLen);
		if((paramPtr!= NULL) && (paramLen != 0))
		{
			/* Remember IR indexes should start from 1. */
			trainIr = strtol(paramPtr, NULL, 10);
			if((trainIr >= 1) && (trainIr <= 3))
			{
				/* Send Train IR Cmd & Wait for completion. */
				if(trainIr == 1)
				{
					gpio_set_level(IR_INT0_PIN, 0);
					vTaskDelay(pdMS_TO_TICKS(IR_TRAIN_DELAY));
					gpio_set_level(IR_INT0_PIN, 1);
				}
				else if(trainIr == 2)
				{
					gpio_set_level(IR_INT1_PIN, 0);
					vTaskDelay(pdMS_TO_TICKS(IR_TRAIN_DELAY));
					gpio_set_level(IR_INT1_PIN, 1);
				}
				else if(trainIr == 3)
				{
					gpio_set_level(IR_INT2_PIN, 0);
					vTaskDelay(pdMS_TO_TICKS(IR_TRAIN_DELAY));
					gpio_set_level(IR_INT2_PIN, 1);
				}
				/* Send OK response. */
				genResp = 1;
			}
			else
			{
				/* Send Failed response. */
				genResp = 2;
			}
		}
		else
		{
			/* Send Failed response. */
			genResp = 2;
		}
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_MAC)) != NULL)
	{
		/* Print MAC-ID of this device. */
		sprintf((char*)outBuff, "\r\nMAC: %s\r\n\r\n", macString);
		*outLen = strlen((char*)outBuff);
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_HELP)) != NULL)
	{
		/* Display help string. */
		strcpy((char*)outBuff, helpStr);
		*outLen = strlen(helpStr);
	}
	else if((cmdStart = strstr((char*)inpBuff, CLI_CMD_RESET)) != NULL)
	{
		/* Issue a device reset command. */
		resetEsp = 1;
		/* Display reset response. */
		*outLen = strlen(CLI_CMD_RESP_DEVRST);
		memcpy(outBuff, CLI_CMD_RESP_DEVRST, *outLen);
	}
	else
	{
		/* Display invalid cmd response. */
		genResp = 3;
	}

	if(genResp == 1)
	{
		/* Send OK response. */
		*outLen = strlen(CLI_CMD_RESP_OK);
		memcpy(outBuff, CLI_CMD_RESP_OK, *outLen);
	}
	else if(genResp == 2)
	{
		/* Send Failed response. */
		*outLen = strlen(CLI_CMD_RESP_FAIL);
		memcpy(outBuff, CLI_CMD_RESP_FAIL, *outLen);
	}
	else if(genResp == 3)
	{
		/* Send Failed response. */
		*outLen = strlen(CLI_CMD_RESP_INVALID);
		memcpy(outBuff, CLI_CMD_RESP_INVALID, *outLen);
	}
}

void vTaskUartCli(void *pvArg)
{
	uart_config_t uartConfig = {};
	uint8_t rxChar = 0;
	int16_t rxIndex = 0;
	uint16_t txCount = 0;

	/* Init UART port. */
	uartConfig.baud_rate = CLI_UART_BAUDRATE;
	uartConfig.data_bits = UART_DATA_8_BITS;
	uartConfig.parity = UART_PARITY_DISABLE;
	uartConfig.stop_bits = UART_STOP_BITS_1;
	uartConfig.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
	uartConfig.rx_flow_ctrl_thresh = 122;

	ESP_ERROR_CHECK(uart_set_pin(CLI_UART, CLI_TX_PIN, CLI_RX_PIN,
					UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	ESP_ERROR_CHECK(uart_param_config(CLI_UART, &uartConfig));

	ESP_ERROR_CHECK(uart_driver_install(CLI_UART, CLI_UART_BUFFSIZE*2,
					0, 0, NULL,
					0));
	ESP_ERROR_CHECK(uart_set_mode(CLI_UART, UART_MODE_UART));

	/* Initialize tempNvsVar variable with correct values. */
	GetCurrentConfig(&tempNvsVar);

	/* Display reset done message. */
	uart_write_bytes(CLI_UART, CLI_MSG_RST_DONE, strlen(CLI_MSG_RST_DONE));

	while(1)
	{
		/* Read 1 char from uart port. */
		rxChar = 0;
		uart_read_bytes(CLI_UART, &rxChar, 1, pdMS_TO_TICKS(50));

		/* If LF detected, process received cmd. */
		if(rxChar == '\r' )
		{
			uart_write_bytes(CLI_UART, (char*)"\r\n", 2);

			if( rxIndex < CLI_UART_BUFFSIZE )
			{
				/* Terminate the input cmd string with null char. */
				uartRxBuff[rxIndex++] = '\0';
			}

			/* Received a command process it. */
			if(rxIndex > 0)
			{
				/* Execute any cmds recvd in UART Rx Queue */
				ExecuteCliCmd(uartRxBuff, uartTxBuff, &txCount);
				if(txCount)
				{
					/* Transmit response buffer. */
					uart_write_bytes(CLI_UART, (char*)uartTxBuff, txCount);
					txCount = 0;
					/* If flag is set, issue a soft reset command. */
					if(resetEsp)
						esp_restart();
				}
				rxIndex = 0;
			}
		}
		else
		{
			if( (rxChar == '\n') || (rxChar == 0))
			{
				/* Ignore line feeds (putty doesn't send LF so modified). */
			}
			else
			{
				if(rxChar == '\b' )
				{
					/* Backspace was pressed.  Erase the last character in the input
					buffer - if there are any. */
					if( rxIndex > 0 )
					{
						rxIndex--;
						uartRxBuff[rxIndex] = '\0';
					}
				}
				else
				{
					/* A character was entered.  It was not a new line, backspace
					or carriage return, so it is accepted as part of the input and
					placed into the input buffer.  When a \n is entered the complete
					string will be passed to the command interpreter. */
					if( rxIndex < CLI_UART_BUFFSIZE )
					{
						uartRxBuff[rxIndex++] = rxChar;
					}
				}
				/* Echo received character. */
				uart_write_bytes(CLI_UART, (char*)&rxChar, 1);
			}
		}
	}
}

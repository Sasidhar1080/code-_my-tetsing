/*
 * applogic.c
 *
 *  Created on: Mar 11, 2020
 *      Author: Mahesh
 */

#include "applogic.h"

#define SBI(var, bit)                   var |= (1<<bit)
#define CBI(var, bit)                   var &= ~(1<<bit)

#define APP_EVT_NRFCONF					BIT0
#define NO_OCCUPANCY            		0xFFFFFFFF
/* NRF comm commands. */
#define NRFCOMM_HEADER               	'$'
#define NRFCOMM_CMD_SET_MAC           	1
#define NRFCOMM_CMD_POLL_STATUS      	2

/* NRF comm error codes. */
#define NRFCOMM_NO_ERR            		0
#define NRFCOMM_LEN_ERR            		-1
#define NRFCOMM_CMD_ERR             	-2
#define NRFCOMM_INV_RESP_ERR          	-3

#define WIFI_LED_BITPOS					0
#define MQTT_LED_BITPOS					1

typedef enum
{
	APPSTATE_NRFRST, APPSTATE_NRFCONFIG, APPSTATE_NRFPOLL
}appStates_t;

struct nrfcomm_tx_packet_t
{
	uint8_t header;
	uint8_t cmdId;
	uint8_t tabLen;
	uint8_t macTable[MAX_MOTION_SNSR_MACIDS][6];
	uint8_t sum;
}__attribute__((packed));
typedef struct nrfcomm_tx_packet_t nrfcomm_tx_packet_t;

extern nvs_const_t nvsVar;
uint8_t sendIrPulse = 0;
uint8_t resendMacs = 0;
uint8_t mqttReadMac = 0;
uint8_t mqttSendStatus = 0;
uint8_t lastCmdAck = 0;
uint32_t occupancyStatus = NO_OCCUPANCY;
float rh = 85.0f, temp = 32.5f;
EventGroupHandle_t appEvtGrp = NULL;
static const char *TAG = "AppLogic";
uint8_t ledStatus = 0;

static void IRAM_ATTR ISRNRFPin(void* arg)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(appEvtGrp != NULL)
	{
		xEventGroupSetBitsFromISR(appEvtGrp, APP_EVT_NRFCONF, &xHigherPriorityTaskWoken);
		if(xHigherPriorityTaskWoken == pdTRUE)
		{
			portYIELD_FROM_ISR();
		}
	}
}

static void ExecuteMqttCmd(mqttDataPkt_t *mqttCmdPtr)
{
	uint16_t i = 0, j = 0;
	nvs_const_t tempNvsVar = {};

	switch(mqttCmdPtr->dataPtr[0])
	{
		case MQTT_CMD_IROFF:
			sendIrPulse = 1;
			lastCmdAck = MQTT_CMD_IROFF;
			mqttSendStatus = 1;
			break;
		case MQTT_CMD_REGMAC:
			/* Payload length should be a multiple of 6 (as 1 MAC ID = 6B) */
			if((mqttCmdPtr->dataPtr[1]%6) == 0)
			{
				/* Store MAC-Table length. */
				tempNvsVar.macTableLen = mqttCmdPtr->dataPtr[1]/6;
				if((tempNvsVar.macTableLen > 0) && (tempNvsVar.macTableLen <= MAX_MOTION_SNSR_MACIDS))
				{
					/* Parse all the MAC-IDs. */
					for(i = 0, j = 2; i < tempNvsVar.macTableLen; i++)
					{
						tempNvsVar.macIdTable[i][0] = mqttCmdPtr->dataPtr[j++];
						tempNvsVar.macIdTable[i][1] = mqttCmdPtr->dataPtr[j++];
						tempNvsVar.macIdTable[i][2] = mqttCmdPtr->dataPtr[j++];
						tempNvsVar.macIdTable[i][3] = mqttCmdPtr->dataPtr[j++];
						tempNvsVar.macIdTable[i][4] = mqttCmdPtr->dataPtr[j++];
						tempNvsVar.macIdTable[i][5] = mqttCmdPtr->dataPtr[j++];
					}
					/* Store received MACs in NVS. */
					SaveConfig(&tempNvsVar, NVSVAR_SAVE_MACTABLE);
					resendMacs = 1;
					lastCmdAck = MQTT_CMD_REGMAC;
					mqttSendStatus = 1;
				}
			}
			break;
		case MQTT_CMD_READMAC:
			mqttReadMac = 1;
			break;
		case MQTT_CMD_READSTATUS:
			mqttSendStatus = 1;
			break;
	}
}

static void SendMqttCmdResp(uint8_t cmdId)
{
	mqttDataPkt_t *mqttTxPtr = calloc(1, sizeof(mqttDataPkt_t));
	uint8_t pktLen = 0;

	switch(cmdId)
	{
		case MQTT_CMD_READMAC:
			/* Check if gateway holds any MAC-IDs. */
			if(nvsVar.macTableLen>0)
			{
				/* Calculate length of the mac table */
				pktLen = (nvsVar.macTableLen*6);
				/* Allocate payload buffer. */
				mqttTxPtr->dataPtr = calloc(pktLen + 2, sizeof(uint8_t));
				assert(mqttTxPtr->dataPtr != NULL);
				/* Form Read MAC response packet. */
				mqttTxPtr->dataPtr[0] = MQTT_CMD_READMAC;
				mqttTxPtr->dataPtr[1] = pktLen;
				memcpy(&mqttTxPtr->dataPtr[2], nvsVar.macIdTable, mqttTxPtr->dataPtr[1]);
				/* MQTT payload length is 2 Bytes extra. */
				mqttTxPtr->dataLen = pktLen + 2;
			}
			else
			{
				/* No MAC-IDs in gateway so send a 2B response packet indicating 0 len. */
				/* Allocate payload buffer. */
				mqttTxPtr->dataPtr = calloc(2, sizeof(uint8_t));
				assert(mqttTxPtr->dataPtr != NULL);
				/* Form Read MAC response packet. */
				mqttTxPtr->dataPtr[0] = MQTT_CMD_READMAC;
				mqttTxPtr->dataPtr[1] = 0;
				mqttTxPtr->dataLen = 2;
			}
			break;
		case MQTT_CMD_READSTATUS:
			/* Allocate 14 Bytes of payload buffer. */
			mqttTxPtr->dataPtr = calloc(14, sizeof(uint8_t));
			assert(mqttTxPtr->dataPtr != NULL);
			/* Form Status response packet. */
			mqttTxPtr->dataPtr[0] = MQTT_CMD_READSTATUS;
			memcpy(&mqttTxPtr->dataPtr[1], &occupancyStatus, sizeof(uint32_t));
			memcpy(&mqttTxPtr->dataPtr[5], &rh, sizeof(float));
			memcpy(&mqttTxPtr->dataPtr[9], &temp, sizeof(float));
			mqttTxPtr->dataPtr[13] = lastCmdAck;
			mqttTxPtr->dataLen = 14;
			break;
	}
	if(MQTTWriteToPubQ(mqttTxPtr, 0) != pdTRUE)
	{
		/* Unable to Write to Queue, so free allocated memory. */
		free(mqttTxPtr->dataPtr);
		free(mqttTxPtr);
	}
}

static uint8_t CalcChkSum(uint8_t *buff, uint8_t len)
{
    uint8_t sum = 0, i = 0;

    while(i < len)
    {
        sum += buff[i];
        i++;
    }
    return sum;
}

static int NRFSendCmdWithResp(uint8_t cmdId)
{
	uint8_t respBuff[8];
	int respLen = 0;
	uint16_t rhInt = 0, tempInt = 0;
	nrfcomm_tx_packet_t pkt = {};

	pkt.header = NRFCOMM_HEADER;

	switch(cmdId)
	{
		case NRFCOMM_CMD_SET_MAC:
			pkt.cmdId = NRFCOMM_CMD_SET_MAC;
			pkt.tabLen = nvsVar.macTableLen;
			memcpy(&pkt.macTable, nvsVar.macIdTable, sizeof(nvsVar.macIdTable));
			break;
		case NRFCOMM_CMD_POLL_STATUS:
			pkt.cmdId = NRFCOMM_CMD_POLL_STATUS;
			break;
		default:
			return NRFCOMM_CMD_ERR;
	}
	/* Update LED status. */
	pkt.cmdId |= (ledStatus << 4);
	pkt.sum = CalcChkSum((uint8_t *)&pkt, sizeof(nrfcomm_tx_packet_t)-1);

	/* Flush all rx data before issuing this cmd. */
	uart_flush(NRF_UART);
	/* Send command. */
	uart_write_bytes(NRF_UART, (char *)&pkt, sizeof(nrfcomm_tx_packet_t));
	respLen = uart_read_bytes(NRF_UART, respBuff, 8, pdMS_TO_TICKS(500));
	if(respLen != 8)
		return NRFCOMM_LEN_ERR;

	switch(cmdId)
	{
		case NRFCOMM_CMD_SET_MAC:
			if(memcmp(respBuff, "OK\r\n\0\0\0\0", 8) != 0)
			{
				return NRFCOMM_INV_RESP_ERR;
			}
			break;
		case NRFCOMM_CMD_POLL_STATUS:
			occupancyStatus = *((uint32_t *)respBuff);
			rhInt = *((uint16_t *)&respBuff[4]);
			tempInt = *((uint16_t *)&respBuff[6]);
			rh = SHT21RhConvIntToFlt(rhInt);
			temp = SHT21TempConvIntToFlt(tempInt);
			break;
	}
	return NRFCOMM_NO_ERR;
}

static void InitPeripherals(void)
{
	gpio_config_t inpConf = {};
	uart_config_t uartConfig = {};
	i2c_config_t i2cConfig = {};

	/* Init NRF RST Pin. */
#ifdef APP_MAKE_RST_HIZ
	/* Init NRF RST pin as input HighZ. */
	inpConf.intr_type = GPIO_PIN_INTR_DISABLE;
	inpConf.mode = GPIO_MODE_INPUT;
	inpConf.pin_bit_mask = (1ULL << NRF_RST_PIN);
	/* Disable internal pull-ups / pull-downs as we have external. */
	inpConf.pull_down_en = 0;
	inpConf.pull_up_en = 0;
	gpio_config(&inpConf);

	/* Init MAXQ6 RST pin as input HighZ. */
	inpConf.pin_bit_mask = (1ULL << IR_RST_PIN);
	gpio_config(&inpConf);
#else
	gpio_pad_select_gpio(NRF_RST_PIN);
	gpio_set_direction(NRF_RST_PIN, GPIO_MODE_OUTPUT);
	gpio_set_drive_capability(NRF_RST_PIN, GPIO_DRIVE_CAP_3);
	gpio_set_level(NRF_RST_PIN, 0);
#endif
	/* Init NRF Int GPIO and ISR. */
	/* Init GPIO interrupt on NRF_INT_PIN */
	inpConf.intr_type = GPIO_PIN_INTR_NEGEDGE;
	inpConf.mode = GPIO_MODE_INPUT;
	inpConf.pin_bit_mask = (1<<NRF_INT_PIN);
	/* Disable internal pull-ups / pull-downs as we have external. */
	inpConf.pull_down_en = 0;
	inpConf.pull_up_en = 0;
	gpio_config(&inpConf);

	/* install gpio isr service */
	gpio_install_isr_service(0);
	gpio_isr_handler_add(NRF_INT_PIN, ISRNRFPin, NULL);

#ifndef APP_MAKE_RST_HIZ
	/* Init IR RST & Int Pins. */
	gpio_pad_select_gpio(IR_RST_PIN);
	gpio_set_direction(IR_RST_PIN, GPIO_MODE_OUTPUT);
	gpio_set_drive_capability(IR_RST_PIN, GPIO_DRIVE_CAP_3);
	gpio_set_level(IR_RST_PIN, 0);
#endif

	gpio_pad_select_gpio(IR_INT0_PIN);
	gpio_set_direction(IR_INT0_PIN, GPIO_MODE_OUTPUT);
	gpio_set_drive_capability(IR_INT0_PIN, GPIO_DRIVE_CAP_3);
	gpio_set_level(IR_INT0_PIN, 1);

	gpio_pad_select_gpio(IR_INT1_PIN);
	gpio_set_direction(IR_INT1_PIN, GPIO_MODE_OUTPUT);
	gpio_set_drive_capability(IR_INT1_PIN, GPIO_DRIVE_CAP_3);
	gpio_set_level(IR_INT1_PIN, 1);

	gpio_pad_select_gpio(IR_INT2_PIN);
	gpio_set_direction(IR_INT2_PIN, GPIO_MODE_OUTPUT);
	gpio_set_drive_capability(IR_INT2_PIN, GPIO_DRIVE_CAP_3);
	gpio_set_level(IR_INT2_PIN, 1);

	/* Init UART for communicating with NRF. */
	uartConfig.baud_rate = NRF_UART_BAUDRATE;
	uartConfig.data_bits = UART_DATA_8_BITS;
	uartConfig.parity = UART_PARITY_DISABLE;
	uartConfig.stop_bits = UART_STOP_BITS_1;
	uartConfig.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
	uartConfig.rx_flow_ctrl_thresh = 122;

	ESP_ERROR_CHECK(uart_set_pin(NRF_UART, NRF_TX_PIN, NRF_RX_PIN,
					UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	ESP_ERROR_CHECK(uart_param_config(NRF_UART, &uartConfig));

	ESP_ERROR_CHECK(uart_driver_install(NRF_UART, NRF_UART_BUFFSIZE*2,
					0, 0, NULL,
					0));
	ESP_ERROR_CHECK(uart_set_mode(NRF_UART, UART_MODE_UART));

	/* Init I2C for communication with SHT21. */
	i2cConfig.mode = I2C_MODE_MASTER;
	i2cConfig.sda_io_num = SDA_PIN;
	i2cConfig.sda_pullup_en = GPIO_PULLUP_DISABLE;
	i2cConfig.scl_io_num = SCL_PIN;
	i2cConfig.scl_pullup_en = GPIO_PULLUP_DISABLE;
	i2cConfig.master.clk_speed = BOARD_I2C_FREQ;
	i2c_param_config(BOARD_I2C, &i2cConfig);
	ESP_ERROR_CHECK(i2c_driver_install(BOARD_I2C, i2cConfig.mode, 0, 0, 0));

	/* Create event group. */
	appEvtGrp = xEventGroupCreate();
}

void vTaskAppLogic(void *pvArg)
{
	mqttDataPkt_t *mqttRxPtr = NULL;

	EventBits_t mqttStatus = 0;
	EventBits_t evtStatus = 0;
	appStates_t state = APPSTATE_NRFRST;
	uint8_t nrfTimeoutCtr = 0;
	uint16_t nrfPollCtr = 0, irOffCtr = 0;

	//use esp_ota_get_app_description() to read project version.

	InitPeripherals();

	/* Remember: Create a timer to poll NRF periodically. */

	/* Reset IR chipset. */
#ifndef APP_MAKE_RST_HIZ
	gpio_set_level(IR_RST_PIN, 0);
	vTaskDelay(pdMS_TO_TICKS(500));
	gpio_set_level(IR_RST_PIN, 1);
#endif
	while(1)
	{

		/* If MAC IDs configured on MQTT / already stored in nvm,
		 * Run occupancy identification state machine.
		 */
		if(nvsVar.macTableLen>0)
		{
			/* Run NRF config / poll state m/c. */
			switch(state)
			{
				case APPSTATE_NRFRST:
					/* Reset NRF */
				#ifndef APP_MAKE_RST_HIZ
					ESP_LOGI(TAG, "NRF RST");
					gpio_set_level(NRF_RST_PIN, 0);
					vTaskDelay(pdMS_TO_TICKS(500));
					gpio_set_level(NRF_RST_PIN, 1);
					vTaskDelay(pdMS_TO_TICKS(500));
				#else
					ESP_LOGI(TAG, "NRF RST BYPASSED");
				#endif
					xEventGroupClearBits(appEvtGrp, APP_EVT_NRFCONF);
					nrfTimeoutCtr = 0;
					occupancyStatus = NO_OCCUPANCY;
					irOffCtr = 0;
					state = APPSTATE_NRFCONFIG;
					break;
				case APPSTATE_NRFCONFIG:
					ESP_LOGI(TAG, "NRF Config");
					/* Wait for NRF pin interrupt to occur. */
					evtStatus = xEventGroupWaitBits(appEvtGrp, APP_EVT_NRFCONF, pdTRUE, pdTRUE, 0);
					if(evtStatus & APP_EVT_NRFCONF)
					{
						/* Send new MAC table to NRF. */
						if(NRFSendCmdWithResp(NRFCOMM_CMD_SET_MAC) == NRFCOMM_NO_ERR)
						{
							state = APPSTATE_NRFPOLL;
							nrfTimeoutCtr = 0;
							/* This is done so that first poll start immediately. */
							nrfPollCtr = NRF_POLL_INTERVAL;
						}
						else
						{
							state = APPSTATE_NRFRST;
						}
					}
					else
					{
						nrfTimeoutCtr++;
						/* Not getting any pulses from NRF, so reset and try again. */
						if(nrfTimeoutCtr >= NRF_CONFIG_TIMEOUT)
						{
							state = APPSTATE_NRFRST;
						}
					}
					break;
				case APPSTATE_NRFPOLL:
					/* This should be done on a periodic timer event. */
					nrfPollCtr++;
					if(nrfPollCtr >= NRF_POLL_INTERVAL)
					{
						ESP_LOGI(TAG, "NRF Poll");
						/* Check with NRF occupancy status of sensors. */
						if(NRFSendCmdWithResp(NRFCOMM_CMD_POLL_STATUS) != NRFCOMM_NO_ERR)
						{
							nrfTimeoutCtr++;
							/* Not getting any response from NRF. */
							if(nrfTimeoutCtr >= 2)
							{
								state = APPSTATE_NRFRST;
							}
						}
						else
						{
							nrfTimeoutCtr = 0;
						}
						/* Reset counter if occupied. */
						if(occupancyStatus != NO_OCCUPANCY)
						{
							irOffCtr = 0;
						}
						/* Send status on MQTT. */
						mqttSendStatus = 1;
						lastCmdAck = 0;
						nrfPollCtr = 0;
					}
					break;
			}
		}

		/* Get MQTTAgent connection status. */
		mqttStatus = WaitMQTTConnect(0);
		/* If MQTT Connected.*/
		if(mqttStatus & MQTT_CONNECT_BIT)
		{
			/* If any cmd in MQTT Rx Queue. */
			if(MQTTReadFromSubQ(&mqttRxPtr, 0) == pdTRUE)
			{
				/* Execute cmd. */
				ExecuteMqttCmd(mqttRxPtr);
				free(mqttRxPtr->dataPtr);
				free(mqttRxPtr);
				mqttRxPtr = NULL;
			}

			if(mqttReadMac)
			{
				SendMqttCmdResp(MQTT_CMD_READMAC);
				mqttReadMac = 0;
			}

			/* On demand status request. */
			if(mqttSendStatus)
			{
				SendMqttCmdResp(MQTT_CMD_READSTATUS);
				mqttSendStatus = 0;
			}
		}
		else
		{
			/* If any cmds in MQTT Rx Queue, clear all cmds. */
			while(!MQTTIsSubQEmpty())
			{
				MQTTReadFromSubQ(&mqttRxPtr, 0);
				free(mqttRxPtr->dataPtr);
				free(mqttRxPtr);
				mqttRxPtr = NULL;
			}
		}

		/* If issue IR off pulse command received on MQTT. */
		/* If all sensors off, issue AC off cmd periodically every minute / 5 minutes. */
		irOffCtr++;
		if((irOffCtr >= IR_OFF_INTERVAL) || (sendIrPulse))
		{
			ESP_LOGI(TAG, "IR OFF");
			/* Generate off cmd pulses on the pins. */
			gpio_set_level(IR_INT0_PIN, 0);
			vTaskDelay(pdMS_TO_TICKS(IR_CMD_PULSE_DELAY));
			gpio_set_level(IR_INT0_PIN, 1);
			sendIrPulse = 0;
			irOffCtr = 0;
		}

		/* If new MAC-IDs received on MQTT. */
		if(resendMacs)
		{
			ESP_LOGI(TAG, "RESEND MACS NRF RST");
			/* Will send new MAC-IDs to NRF. */
			state = APPSTATE_NRFRST;
			resendMacs = 0;
		}

		/* If No cmds pending in MQTT Rx Queue, delay */
//		if(MQTTIsSubQEmpty())
//		{
//		}
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void SetWifiLedState(uint8_t state)
{
	if(state)
	{
		SBI(ledStatus, WIFI_LED_BITPOS);
	}
	else
	{
		CBI(ledStatus, WIFI_LED_BITPOS);
	}
}

void SetMqttLedState(uint8_t state)
{
	if(state)
	{
		SBI(ledStatus, MQTT_LED_BITPOS);
	}
	else
	{
		CBI(ledStatus, MQTT_LED_BITPOS);
	}
}

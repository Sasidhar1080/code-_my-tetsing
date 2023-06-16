/*
 * scrc_flowmeter.cpp
 *
 *  Created on: Dec 31, 2020
 *      Author: suhas
 */

#include "scrc_constants.h"
#if defined(WM_WF)
#include "scrc_flowmeter.h"
#include <Arduino.h>
#include <esp32ModbusRTU.h>

long int *WF_buffer = new long int[WF_REG_LEN / 2];
uint8_t *WF_buffer_total = new uint8_t[4];
long int *WF_buffer_total_int = new long int[1];

long int FlowRate;
long int TotalFlow;
static esp32ModbusRTU wf_modbus(&Serial1, WF_UART_RTS);

INIT_ARR(WF_ADDRESS, (WF_ADDRESS_1,WF_ADDRESS_2,WF_ADDRESS_3,WF_ADDRESS_4));
void hw_setup_flowmeter() {
	//modbus setup
	Serial1.begin(WF_UART_BAUD_RATE, WF_UART_CONFIG, WF_UART_RX, WF_UART_TX);

	/*
	 * Flow Meter-1
	 */
	wf_modbus.onData(
			[](uint8_t serverAddress, esp32Modbus::FunctionCode fc,
					uint8_t *data, size_t length) {
				//Serial.printf("id 0x%02x fc 0x%02x len %u: 0x", serverAddress, fc, length);

				uint8_t *buffer = new uint8_t[length];

				for (size_t i = 0; i < length; ++i) {
					(i % 2 == 0) ?
							buffer[i] = data[i + 1] : buffer[i] = data[i - 1];
					// Serial.printf("\ndata: %02x ", data[i]);

					//Serial.printf("\nbuffer :%02x ", buffer[i]);
					//Serial.printf("---%02d",i);
				}
				Serial.printf(" DONE\n");

				int k = 0;

				for (int j = 12; j <= 15; j++) {
					Serial.printf("\nBuffer : %02x --j=%d", buffer[j], j);
					WF_buffer_total[k] = buffer[j];
					k++;
				}
				WF_buffer = (long int*) buffer;
				WF_buffer_total_int = (long int*) WF_buffer_total;

				Serial.printf("WF_FLOW_RATE: %ld", WF_buffer[WF_FLOW_RATE]);
				Serial.printf(", WF_TOTAL_FLOW: %ld", WF_buffer_total_int[0]);
				FlowRate = WF_buffer[WF_FLOW_RATE];
				TotalFlow = WF_buffer_total_int[0];
				debug_info("WaterFlow Values assigned");
			});

	wf_modbus.onError([](esp32Modbus::Error error) {

		Serial.printf("WF error: 0x%02x\n", static_cast<uint8_t>(error));
	});

	wf_modbus.begin();
}
void hw_read_flowmeter(double *flowrate_buf, double *totalflow_buf,
		int node_id) {
	Serial.println("sending WaterFlow Modbus request...\n");
	static int x = 1;
	if (x == 1)
		hw_setup_flowmeter();
	if (node_id == 0)
		wf_modbus.readHoldingRegisters(WF_ADDRESS_1, WF_REG_ADDR,
		WF_REG_LEN);

	else if (node_id == 1)
		wf_modbus.readHoldingRegisters(WF_ADDRESS_2, WF_REG_ADDR,
		WF_REG_LEN);
	else if (node_id == 2)
		wf_modbus.readHoldingRegisters(WF_ADDRESS_3, WF_REG_ADDR,
		WF_REG_LEN);
	else if (node_id == 3)
		wf_modbus.readHoldingRegisters(WF_ADDRESS_4, WF_REG_ADDR,
		WF_REG_LEN);
	else
		debug_info("Wrong NOde id");

	delay(10000);

	*flowrate_buf = FlowRate;
	*totalflow_buf = TotalFlow;
	FlowRate = 0;
	TotalFlow = 0;
	x = 2;
}
#endif

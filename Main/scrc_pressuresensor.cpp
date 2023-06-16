/*
 * scrc_pressuresensor.cpp
 *
 *  Created on: Jan 29, 2021
 *      Author: suhas
 */
#include "scrc_constants.h"
#if defined(WM_WF)

INIT_ARR(PRESSSURE,
		(PRESSURE_PIN_1,PRESSURE_PIN_2,PRESSURE_PIN_3,PRESSURE_PIN_4));
#include "scrc_pressuresensor.h"
#include <Arduino.h>
#include <esp32ModbusRTU.h>

namespace {

const double ESP32_ADC_MAX_RESOLUTION = 4096.0;

}
static esp32ModbusRTU ps_modbus(&Serial1, PS_UART_RTS);
int pressure;
long int *ps_buffer = new long int[PS_REG_LEN / 2];

#define VREF 3.3

#define BUFFERCOUNT 30

void hw_setup_pressuresensor() {
	if(NO_OF_NODES == 1)
	pinMode(PRESSURE_PIN_1, INPUT);
	else{
		pinMode(PRESSURE_PIN_1, INPUT);
		pinMode(PRESSURE_PIN_2, INPUT);
		pinMode(PRESSURE_PIN_3, INPUT);
		pinMode(PRESSURE_PIN_4, INPUT);
	}

#ifdef PRESSURE_MODBUS
	ps_modbus.onData(
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
				int x, y, z;
				x = data[34];
				y = data[35];

				z = x << 8 | y;
				//ps_buffer = (int*) buffer;

				Serial.printf("Pressure: %d", z);
				pressure = z;

			});

	ps_modbus.onError([](esp32Modbus::Error error) {
		Serial.printf("PS error: 0x%02x\n", static_cast<uint8_t>(error));
	});

	ps_modbus.begin();
#endif
}
void hw_read_pressuresensor(double *pressure_buf, double *pressurevoltage_buf,
		int node_id) {

	static int x = 1;
	if (x == 1)
		hw_setup_pressuresensor();
#ifdef PRESSURE_MODBUS
	Serial.println("sending PressureSensor Modbus request...\n");
	ps_modbus.readHoldingRegisters(PS_ADDRESS, PS_REG_ADDR, PS_REG_LEN);
	delay(10000);
	*pressure_buf = pressure;
#endif

	x = 2;
	double analogVoltage = 0;

	analogVoltage += analogRead(PRESSSURE[node_id]) * VREF
			/ ESP32_ADC_MAX_RESOLUTION;

	double averagevoltage = analogVoltage / BUFFERCOUNT;
	Serial.println(averagevoltage);
	*pressurevoltage_buf = averagevoltage;
}
#endif

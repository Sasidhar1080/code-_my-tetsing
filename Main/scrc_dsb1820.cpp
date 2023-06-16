/*
 * scrc_DSB1820.ino
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include "scrc_constants.h"
#if defined(WM_WD_RO) || defined(WM_WD_OH_ALL)
#include <OneWire.h>
#include "scrc_dsb182011.h"
static OneWire ds(TEMP_SENSOR_PIN);
/*
 * Temperature Setup Function
 */
void hw_setup_temp() {

	debug_info("Inside Temperature Setup");
	ds.reset();
	float temp123;
	hw_read_temp(&temp123);
}

/*
 * Temperature read Function
 */

float hw_read_temp(float *buf_temp) {

	byte data[12];
	byte addr[8];

	if (!ds.search(addr)) {
		ds.reset_search();
		debug_info("Device Not Found");
		return E_DSB_SEARCH;
	}

	if (OneWire::crc8(addr, 7) != addr[7]) {
		debug_info("CRC is not valid!");
		return E_DSB_CRC;
	}

	if (addr[0] != 0x10 && addr[0] != 0x28) {
		debug_info("Device is not recognized");
		return E_DSB_DEVICE;
	}
	// start conversion, with parasite power on at the end
	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);

	usleep(3000);

	// Read Scratch pad
	ds.reset();
	ds.select(addr);
	ds.write(0xBE);

	usleep(3000);

	// we need 9 bytes
	for (int i = 0; i < 9; i++) {
		data[i] = ds.read();
	}

	ds.reset_search();

	byte MSB = data[1];
	byte LSB = data[0];

	float tempRead = ((MSB << 8) | LSB); //using two's compliment
	float TemperatureSum = tempRead / 16;

	*buf_temp = TemperatureSum;
	debug_info(*buf_temp);
	ds.reset();
	return TemperatureSum;
}

#endif

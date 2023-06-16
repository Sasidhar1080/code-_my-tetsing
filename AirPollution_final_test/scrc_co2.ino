/*
 * scrc_co2.c
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */
#include <Arduino.h>
#include "HardwareSerial.h"

#include <Wire.h>

#include "scrc_co2.h"

#define MIN_READING 0
#define MAX_READING 2000

//#define version "T6700-R15 I2C v1.0"
#define ADDR_6700  0x15 // default I2C slave address is 0x15

int readco2();

//float data1 [6];
//float CO2ppmValue;

void hw_setup_co2() {

}

void hw_read_co2(float *buf_co2) {
	*buf_co2 = readco2();
	if (*buf_co2 == -256 || *buf_co2 < MIN_READING || *buf_co2 > MAX_READING) {
		*buf_co2 = NAN;
	}

	Serial.print("......CO2 :");
	Serial.println(*buf_co2);

}

int readco2() {
	float data1[6];
	float CO2ppmValue;
	// start I2C
	Wire.beginTransmission(ADDR_6700);
	Wire.write(0x04);
	Wire.write(0x13);
	Wire.write(0x8B);
	Wire.write(0x00);
	Wire.write(0x01);
	// end transmission
	Wire.endTransmission();
	// read report of current gas measurement in ppm
	delay(2000);
	Wire.requestFrom(ADDR_6700, 5);    // request 4 bytes from slave device
	data1[0] = Wire.read();
	data1[1] = Wire.read();
	data1[2] = Wire.read();
	data1[3] = Wire.read();
	return CO2ppmValue = ((data1[2] * 0xFF) + data1[3]);

}

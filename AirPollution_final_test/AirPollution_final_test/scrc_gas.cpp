/*
 * scrc_gas.cpp
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */

#include <MiCS6814-I2C.h>

#define MIN_CO_READING 1
#define MAX_CO_READING 1000           // value in ppm

#define MIN_NH3_READING 1
#define MAX_NH3_READING 500

#define MIN_NO2_READING 0.05
#define MAX_NO2_READING 10

MiCS6814 sensor;
bool sensorConnected;

void hw_setup_gas() {
	sensorConnected = sensor.begin();

	if (sensorConnected == true) {
		// Print status message
		Serial.println("Connected to MiCS-6814 sensor");

		// Turn heater element on
		sensor.powerOn();
	}

	else {
		// Print error message on failed connection
		Serial.println("Couldn't connect to MiCS-6814 sensor");
	}
}

void hw_read_gas(float *buf_co, float *buf_no2, float *buf_nh3) {
	float co = sensor.measureCO();
	*buf_co = co;
	if (*buf_co == -1 || *buf_co < MIN_CO_READING || *buf_co > MAX_CO_READING) {
		*buf_co = NAN;
	}
	Serial.print("......CO :");
	Serial.print(*buf_co);
	Serial.print("\t");

	float no2 = sensor.measureNO2();
	*buf_no2 = no2;
	if (*buf_no2
			== -1|| *buf_no2 < MIN_NO2_READING || *buf_no2 > MAX_NO2_READING) {
		*buf_no2 = NAN;
	}
	Serial.print("......NO2 :");
	Serial.print(*buf_no2);
	Serial.print("\t");

	float nh3 = sensor.measureNH3();
	*buf_nh3 = nh3;

	if (*buf_nh3
			== -1|| *buf_nh3 < MIN_NH3_READING || *buf_nh3 > MAX_NH3_READING) {
		*buf_nh3 = NAN;
	}
	Serial.print("......NH3 :");
	Serial.println(*buf_nh3);

}

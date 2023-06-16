/*
 * scrc_TDS.ino
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#include <Arduino.h>
#include "scrc_constants.h"
#if defined(WM_WD_RO) || defined(WM_WD_OH_ALL)
#include "scrc_tds.h"
#include "scrc_dsb182011.h"

#define VREF 3.3
#define BufferCount 30

void hw_setup_tds() {
	pinMode(TDS_SENSOR_PIN, INPUT);
}

void hw_read_tds(double *buf_tdsv, float *buf_tdswithouttemp,
		float *buf_tdswithtemp) {

	float Voltage = 0;
	float averageVoltage = 0;
	float tdsValue = 0;
	float temperature = 0;
	float analogVoltage = 0;
	float temp1 = 0;

	//Read Current Water Temperature
	temperature = hw_read_temp(&temp1);

	//store  input voltages in a buffer
	for (int i = 0; i < BufferCount; i++) {
		analogVoltage = analogRead(TDS_SENSOR_PIN) * (float) VREF / 4096.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
		Voltage += analogVoltage;
		delay(100);
	}
	//average the input voltages
	averageVoltage = Voltage / BufferCount;

	*buf_tdsv = averageVoltage;

	//TDS with Temperature Compensation
	float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
	float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation

	tdsValue = (133.42 * compensationVolatge * compensationVolatge
			* compensationVolatge
			- 255.86 * compensationVolatge * compensationVolatge
			+ 857.39 * compensationVolatge) * 0.5;
	*buf_tdswithtemp = tdsValue;

	//TDS without Temperature Compensation
	float compensationCoefficient_without_temp = 1.0 + 0.02 * (25.0 - 25.0);
	float compensationVolatge_withouttemp = averageVoltage
			/ compensationCoefficient_without_temp;
	*buf_tdswithouttemp = (133.42 * compensationVolatge_withouttemp
			* compensationVolatge_withouttemp * compensationVolatge_withouttemp
			- 255.86 * compensationVolatge_withouttemp
					* compensationVolatge_withouttemp
			+ 857.39 * compensationVolatge_withouttemp) * 0.5;

	//printing the TDS values
	debug_info("TDS Value: " + String(tdsValue) + " ppm");

}
#endif

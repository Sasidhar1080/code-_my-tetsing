/*
 * scrc_turbidity.cpp
 *
 *  Created on: 26-Jan-2021
 *      Author: Usha
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include "scrc_constants.h"
#if defined(WM_WD_OH) ||defined(WM_WD_OH_ALL)|| defined(WM_WD_RO)
#include "scrc_turbidity.h"
#define turbiditySensorPin 34
float rawVoltage = 0 , voltage = 0, turbidity =0;

void hw_setup_turbidity() {
//this is empty
	pinMode(turbiditySensorPin, INPUT);
	debug_info("Inside Turbidity Setup");
}

void hw_read_turbidity(float *buf_turbidity) {
	rawVoltage = analogRead(turbiditySensorPin) * (3.3 / 4096.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
	Serial.println("Turbidity:");
	Serial.print(voltage); // print out the value you read:
	turbidity = map(rawVoltage, 0, 3.3, 0, 5);
	Serial.print(turbidity); // print out the value you read:

	*buf_turbidity = turbidity;
	delay(500);
}
#endif

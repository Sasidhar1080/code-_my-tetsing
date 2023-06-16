/*
 * scrc_pH.cpp
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include "scrc_constants.h"
#if defined(WM_WD_OH) ||defined(WM_WD_OH_ALL)|| defined(WM_WD_RO)
#include "scrc_pH.h"
#define pHSensorPin 14
#define Offset 0.00
#define samplingInterval 20
#define ArrayLenth  40    //times of collection
double avergearray(int *arr, int number);
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex = 0;

void hw_setup_pH() {
	pinMode(pHSensorPin, INPUT);
}

void hw_read_pH(float *buf_pH) {

	float rawVoltage = 0;
	static float pHValue, voltage;
	for (int i = 0; i < samplingInterval; i++) {
		rawVoltage = analogRead(pHSensorPin) * 3.3 / 4095;
		Serial.print(rawVoltage);
		pHArray[pHArrayIndex++] = analogRead(pHSensorPin);
		if (pHArrayIndex == ArrayLenth)
			pHArrayIndex = 0;

		voltage = avergearray(pHArray, ArrayLenth) * 3.3 / 4095;
	}
	pHValue = -5.480 * voltage + 15.132;
	//pHValue = 3.5*voltage+Offset;
	Serial.print("Voltage:");
	Serial.print(voltage);
	Serial.print("pHValue:");
	Serial.println(pHValue);
	if(pHValue<=0){
		pHValue=6.9;
	}
	*buf_pH = pHValue;
	Serial.print(*buf_pH);

}

double avergearray(int *arr, int number) {
	int i;
	int max, min;
	double avg;
	long amount = 0;
	if (number <= 0) {
		Serial.println("Error number for the array to avraging!/n");
		return 0;
	}
	if (number < 5) {   //less than 5, calculated directly statistics
		for (i = 0; i < number; i++) {
			amount += arr[i];
		}
		avg = amount / number;
		return avg;
	} else {
		if (arr[0] < arr[1]) {
			min = arr[0];
			max = arr[1];
		} else {
			min = arr[1];
			max = arr[0];
		}
		for (i = 2; i < number; i++) {
			if (arr[i] < min) {
				amount += min;        //arr<min
				min = arr[i];
			} else {
				if (arr[i] > max) {
					amount += max;    //arr>max
					max = arr[i];
				} else {
					amount += arr[i]; //min<=arr<=max
				}
			} //if
		} //for
		avg = (double) amount / (number - 2);
	} //if
	return avg;
}
#endif

/*
 * scrc_aql.c
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */

#include <Arduino.h>
#include <HardwareSerial.h>

#include "scrc_aql.h"

enum levels level;

short int aql(float pm) {
	if (pm <= 50)
		return level = Good;

	else if (pm > 50 && pm <= 100)
		return level = Satisfactory;

	else if (pm > 100 && pm <= 200)
		return level = Moderate;

	else if (pm > 200 && pm <= 300)
		return level = Poor;

	else if (pm > 300 && pm <= 400)
		return level = Very_Poor;

	else if (pm > 400 && pm <= 500)
		return level = Severe;
	else
		return level = Unknown;

}

short int aqi_mp(float pm25, float pm10) {

	if (pm25 > pm10)
		return p25;
	else
		return p10;
}

float compute_index_aqi(float p25, float p10)

{

	float s10, s25;

	if (p10 <= 100) {

		s10 = p10;

	} else if (p10 > 100 && p10 <= 250) {

		s10 = 100 + ((p10 - 100) * 100 / 150);

	} else if (p10 > 250 && p10 <= 350) {

		s10 = 200 + ((p10 - 250));

	} else if (p10 > 350 && p10 <= 430) {

		s10 = 300 + ((p10 - 350) * 100 / 80);

	} else {

		s10 = 400 + ((p10 - 430) * 100 / 80);

	}

	if (p25 <= 30) {

		s25 = p25 * 50 / 30;

	} else if (p25 > 30 && p25 <= 60) {

		s25 = 50 + ((p25 - 30) * 50 / 30);

	} else if (p25 > 60 && p25 <= 90) {

		s25 = 100 + ((p25 - 60) * 100 / 30);

	} else if (p25 > 90 && p25 <= 120) {

		s25 = 200 + ((p25 - 90) * 100 / 30);

	} else if (p25 > 120 && p25 <= 250) {

		s25 = 300 + ((p25 - 120) * 100 / 130);

	} else {

		s25 = 400 + ((p25 - 250) * 100 / 130);

	}

	if (s10 > s25)

	{
		Serial.print("AQI: ");
		Serial.println(s10);

		return s10;

	}

	else {
		Serial.print("AQI: ");
		Serial.println(s25);
		return s25;

	}

}

/*
 * scrc_noise.cpp
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */
#include <Arduino.h>

#include "scrc_noise.h"

#define MIN_NOISE_READING 48
#define MAX_NOISE_READING 100

const int sampleWindow = 50;
unsigned int sample;

void hw_setup_noise() {

}

void hw_read_noise(short int *buf_noise) {

	unsigned long startMillis = millis();              // Start of sample window
	short int peakToPeak = 0;                              // peak-to-peak level

	unsigned int signalMax = 0;                            //minimum value
	unsigned int signalMin = 1024;                         //maximum value

	// collect data for 50 mS
	while (millis() - startMillis < sampleWindow) {
		sample = analogRead(A0);                   //get reading from microphone
		if (sample < 1024)                         // toss out spurious readings
				{
			if (sample > signalMax) {
				signalMax = sample;                  // save just the max levels
			} else if (sample < signalMin) {
				signalMin = sample;                  // save just the min levels
			}
		}
	}
	peakToPeak = signalMax - signalMin;       // max - min = peak-peak amplitude
	short int noise = map(peakToPeak, 20, 900, 49.5, 90); //calibrate for deciBels
	*buf_noise = noise;
	if (*buf_noise
			== 49|| *buf_noise < MIN_NOISE_READING || *buf_noise > MAX_NOISE_READING) {
		*buf_noise = NAN;
	}
	Serial.print("......Noise Value: ");
	Serial.println(*buf_noise);

}

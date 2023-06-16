/*
 * scrc_level.ino
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */


#include <Arduino.h>
#include <HardwareSerial.h>
#include "scrc_constants.h"

#if defined(WM_WD_OH) ||defined(WM_WD_OH_ALL) || defined(WM_WD_RO)
#include "scrc_level.h"

/*
 * Level Sensor Setup
 */
void hw_setup_level() {
	pinMode(ULTRASONIC_SENSOR_TRIGGER_PIN, OUTPUT);
	pinMode(ULTRASONIC_SENSOR_ECHO_PIN, INPUT);
}

/*
 * Hardware Read data from Level Sensor
 */
void hw_read_level(double *buf_level) {
	double distance = sense_level();
	*buf_level = distance;
	delay(2000);
}
/*
 * Level Sensor function
 */
double sense_level() {

	long duration = 0; // variable for the duration of sound wave travel
	double dist;
	// Clears the trigPin condition
	digitalWrite(ULTRASONIC_SENSOR_TRIGGER_PIN, LOW);
	delayMicroseconds(2);
	// Sets the trigPin HIGH (ACTIVE) for 10 microseconds
	digitalWrite(ULTRASONIC_SENSOR_TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(ULTRASONIC_SENSOR_TRIGGER_PIN, LOW);
	// Reads the echoPin, returns the sound wave travel time in microseconds
	duration = pulseIn(ULTRASONIC_SENSOR_ECHO_PIN, HIGH);
	// Calculating the distance
	dist = (duration) * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
	// Displays the distance on the Serial Monitor
	debug_info("Distance: " + String(dist) + " cms");
	//if (dist > CONTAINER_HEIGHT || dist < 0) {
	//delay(500);
	//return sense_level();

	return dist;

}
#endif

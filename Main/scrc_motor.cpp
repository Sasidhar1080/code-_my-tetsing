/*
 * scrc_motor.ino
 *
 *  Created on: Nov 22, 2020
 *      Author: suhas
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include "scrc_constants.h"
#if defined(WM_WD_RO)||defined(WM_WD_OH_ALL)
#include "scrc_motor.h"
#include "scrc_level.h"

/*
 * Input Motor Setup
 */
void hw_setup_motor_in() {

	pinMode(MOTOR_IN_PIN, OUTPUT);
	digitalWrite(MOTOR_IN_PIN, MOTOR_IN_OFF);
	delay(500);
}

/*
 * Output Motor Setup
 */
void hw_setup_motor_out() {
	pinMode(MOTOR_OUT_PIN, OUTPUT);
	digitalWrite(MOTOR_OUT_PIN, MOTOR_OUT_OFF);
	delay(500);
}

/*
 * Set Input Motor
 */
void hw_set_motor_in(int motor_in_mode) {
	if (motor_in_mode == 0) {

		digitalWrite(MOTOR_IN_PIN, MOTOR_IN_OFF);
		debug_info("Input Motor Switched OFF ");
	}

	else if (motor_in_mode == 1) {

		digitalWrite(MOTOR_IN_PIN, MOTOR_IN_ON);
		debug_info("Input Motor Switched ON ");
	} else {
		//Any other Functionality if needed
		debug_info("Entered Special Functionality Mode");

	}

}
/*
 * Set Output Motor
 */
void hw_set_motor_out(int motor_out_mode) {
	if (motor_out_mode == 0) {

		digitalWrite(MOTOR_OUT_PIN, MOTOR_OUT_OFF);
		debug_info("Output Motor Switched OFF ");
	}

	else if (motor_out_mode == 1) {
		digitalWrite(MOTOR_OUT_PIN, MOTOR_OUT_ON);
		debug_info("Output Motor Switched ON ");
	} else {
		//Any other Functionality if needed
		debug_info("Entered Special Functionality Mode");
	}
}

/*
 * Container Rinse Function
 */
void hw_set_motor_rinse(int rinse_count) {
	debug_info("Rinsing the Container");
	for (int i = 0; i < rinse_count; i++) {
		/*
		 while(CONTAINER_HEIGHT-BUFFER_HEIGHT-sense_level()>0)
		 {
		 hw_set_motor_in(0);
		 }
		 hw_set_motor_in(0);

		 while(CONTAINER_HEIGHT-sense_level()>0)
		 {
		 hw_set_motor_out(0);
		 }
		 delay(MOTOR_OUT_BUFFER_TIME);
		 hw_set_motor_out(1);

		 while(CONTAINER_HEIGHT-BUFFER_HEIGHT-sense_level()>0)
		 {
		 hw_set_motor_in(0);
		 }
		 hw_set_motor_in(0);

		 while(CONTAINER_HEIGHT-sense_level()>0)
		 {
		 hw_set_motor_out(0);
		 }
		 delay(MOTOR_OUT_BUFFER_TIME);
		 hw_set_motor_out(1);

		 debug_info("Rinse Count : " + (i + 1));


		 */
		hw_set_motor_out(1);
		delay(MOTOR_OUTPUT_ON_INTERVAL);
		hw_set_motor_out(0);
		delay(1000);
		hw_set_motor_in(1);
		delay(MOTOR_INPUT_ON_INTERVAL);
		hw_set_motor_in(0);
		delay(200);
		hw_set_motor_out(1);
		delay(MOTOR_OUTPUT_ON_INTERVAL);
		hw_set_motor_out(0);
		delay(1000);
		debug_info("Rinse Count : " + (i + 1));
		delay(1000);
	}

	/* while(CONTAINER_HEIGHT-BUFFER_HEIGHT-sense_level()>0)
	 {
	 hw_set_motor_in(0);
	 }
	 hw_set_motor_in(0);

	hw_set_motor_in(1);
	delay(MOTOR_INPUT_ON_INTERVAL);
	hw_set_motor_in(0);
	delay(5000);*/
	/*hw_set_motor_out(1);
    delay(MOTOR_OUTPUT_ON_INTERVAL);
	hw_set_motor_out(0);
	delay(200);*/
}
/*
 * Container Clean Function
 */
void hw_set_motor_clean() {
	debug_info("Cleaning the Container");
	while (CONTAINER_HEIGHT - CLEAN_HEIGHT - sense_level() > 0) {
		hw_set_motor_in(0);
	}
	hw_set_motor_in(0);

	while (CONTAINER_HEIGHT - sense_level() > 0) {
		hw_set_motor_out(0);
	}
	delay(MOTOR_OUT_BUFFER_TIME);
	hw_set_motor_out(1);

	while (CONTAINER_HEIGHT - CLEAN_HEIGHT - sense_level() > 0) {
		hw_set_motor_in(0);
	}
	hw_set_motor_in(0);

	while (CONTAINER_HEIGHT - sense_level() > 0) {
		hw_set_motor_out(0);
	}
	delay(MOTOR_OUT_BUFFER_TIME);
	hw_set_motor_out(1);
}
#endif

/*
 * scrc_motor.h
 *
 *  Created on: Dec 20, 2020
 *      Author: suhas
 */

#ifndef SCRC_MOTOR_H_
#define SCRC_MOTOR_H_


void hw_setup_motor_in();
void hw_setup_motor_out();

void hw_set_motor_in(int motor_in_mode);
void hw_set_motor_out(int motor_out_mode);

void hw_set_motor_rinse(int rinse_count);
void hw_set_motor_clean();

#endif /* SCRC_MOTOR_H_ */

/*
 * scrc_gas.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */

#ifndef INCLUDE_SCRC_GAS_H_
#define INCLUDE_SCRC_GAS_H_

void hw_setup_gas();
void hw_read_gas(float *buf_co, float *buf_no2, float *buf_nh3);

#endif /* INCLUDE_SCRC_GAS_H_ */

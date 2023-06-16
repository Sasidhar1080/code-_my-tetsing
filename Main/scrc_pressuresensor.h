/*
 * scrc_pressuresensor.h
 *
 *  Created on: Jan 29, 2021
 *      Author: suhas
 */

#ifndef SCRC_PRESSURESENSOR_H_
#define SCRC_PRESSURESENSOR_H_


void hw_setup_pressuresensor();
void hw_read_pressuresensor(double *pressure_buf, double *pressurevoltage_buf,
		int node_id);

#endif /* SCRC_PRESSURESENSOR_H_ */

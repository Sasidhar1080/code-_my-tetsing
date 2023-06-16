/*
 * scrc_flowmeter.h
 *
 *  Created on: Dec 31, 2020
 *      Author: suhas
 */

#ifndef SCRC_FLOWMETER_H_
#define SCRC_FLOWMETER_H_


void hw_setup_flowmeter();
void hw_read_flowmeter(double *flowrate_buf, double *totalflow_buf,
		int node_id);

#endif /* SCRC_FLOWMETER_H_ */


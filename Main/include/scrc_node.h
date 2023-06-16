/*
 * scrc_node.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#ifndef SCRC_NODE_H_
#define SCRC_NODE_H_

#include <stdint.h>
#include "scrc_constants.h"

struct sensors_data {
	uint64_t time_stamp;

#ifdef NT_WATER
	/*
	 Water monitoring related sensor

	 */

#if defined(WM_WD_RO) ||defined(WM_WD_OH_ALL)
	float temp;
	double tdsvoltage;
	float tdswithouttemp;
	float tdswithtemp;


#endif

#if defined(WM_WD_OH) || defined(WM_WD_OH_ALL)
	float turbidity;
	float pH;

#endif

#if defined(WM_WD_OH) ||defined(WM_WD_OH_ALL)
	double level;

#endif

#if defined(WM_WF)

	double flowrate;
	double totalflow;
	double pressure;
	double pressurevoltage;

#endif

#endif

	int node_id;

//short int_padding_1; // padding is based on the no of parameters and sizeof each parameter
};

#endif /* SCRC_NODE_H_ */


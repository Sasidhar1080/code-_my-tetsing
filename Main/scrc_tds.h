/*
 * scrc_tds.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#ifndef SCRC_TDS_H_
#define SCRC_TDS_H_

void hw_setup_tds();

void hw_read_tds(double *buf_tdsv, float *buf_tdswithouttemp,
		float *buf_tdswithtemp);

#endif /* SCRC_TDS_H_ */

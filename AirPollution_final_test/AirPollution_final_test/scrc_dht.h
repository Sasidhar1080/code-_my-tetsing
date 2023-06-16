/*
 * scrc_dht.h
 *
 *  Created on: 12-May-2022
 *      Author: Ayu Parmar
 */

#ifndef INCLUDE_SCRC_AHT_H_
#define INCLUDE_SCRC_AHT_H_

void hw_setup_aht();
void hw_read_aht(float *buf_temp, float *buf_rh);

#endif /* INCLUDE_SCRC_AHT_H_ */

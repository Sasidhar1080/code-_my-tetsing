/*
 * scrc_sds.h
 *
 *  Created on: 12-May-2022
 *      Author: Ayu Parmar
 */

#ifndef INCLUDE_SCRC_SDS_H_
#define INCLUDE_SCRC_SDS_H_

void hw_setup_sds();
void hw_read_sds(float *buf_pm25, float *buf_pm10);

#endif /* INCLUDE_SCRC_SDS_H_ */

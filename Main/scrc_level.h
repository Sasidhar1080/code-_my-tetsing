/*
 * scrc_level.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#ifndef SCRC_LEVEL_H_
#define SCRC_LEVEL_H_


void hw_setup_level();
double sense_level();
void hw_read_level(double *buf_level);

#endif /* SCRC_LEVEL_H_ */

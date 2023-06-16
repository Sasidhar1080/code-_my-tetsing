/*
 * scrc_wifi.h
 *
 *  Created on: 12-May-2022
 *      Author: Ayu Parmar
 */

#ifndef INCLUDE_SCRC_WIFI_H_
#define INCLUDE_SCRC_WIFI_H_

void nw_setup();
bool nw_start(unsigned long timeout, int retry_delay);
void nw_stop();

bool nw_is_connected();

#endif /* INCLUDE_SCRC_WIFI_H_ */

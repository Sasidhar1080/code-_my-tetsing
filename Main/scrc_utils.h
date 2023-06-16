/*
 * scrc_utils.h
 *
 *  Created on: 22-Apr-2021
 *      Author: DELL
 */
#pragma once
#ifndef SCRC_UTILS_H_
#define SCRC_UTILS_H_
#include<Arduino.h>

#include "scrc_constants.h"
#include "scrc_node.h"
#include "scrc_wifi.h"
#include "scrc_time.h"
#include "scrc_http.h"
#include "scrc_pub_onem2m.h"
#include "scrc_pub_thingspeak.h"

int retry_wifi();
bool is_om2m_pub_due(unsigned long milli);
bool is_thngspk_pub_due(unsigned long milli);
void update_hist_idx_thngspk(int pub_count);
void update_hist_idx_om2m(int pub_count);


#endif /* SCRC_UTILS_H_ */

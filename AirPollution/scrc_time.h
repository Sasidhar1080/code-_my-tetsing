/*
 * scrc_time.h
 *
 *  Created on: 12-May-2022
 *      Author: Ayu Parmar
 */

#ifndef INCLUDE_SCRC_TIME_H_
#define INCLUDE_SCRC_TIME_H_

void sync_time();
uint64_t get_timestamp();
uint64_t get_timestamp(unsigned long milli);

String get_timestamp_str(uint64_t timestamp, uint16_t timeoffset);
String get_datestamp_str(uint64_t timestamp, uint16_t timeoffset);

String get_date_timestamp_str(uint64_t timestamp, uint16_t timeoffset);

#endif /* INCLUDE_SCRC_TIME_H_ */

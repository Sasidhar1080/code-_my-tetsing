/*
 * scrc_utils.cpp
 *
 *  Created on: 22-Apr-2021
 *      Author: DELL
 */
#include "scrc_constants.h"
#include "scrc_utils.h"

extern short int hist_idx_om2m;

extern short int hist_idx_thngspk;

extern  bool wifi_connected;

/*
 *Timing Variables
 */

extern  uint32_t last_om2m_pub;

extern  uint32_t last_thngspk_pub;

extern  uint32_t last_retry_wifi;

extern  uint32_t retry_wifi_interval;


int retry_wifi() {
	//regular interval with incremental back-off try to reconnect WiFi

	unsigned long milli = millis();
	if (milli - last_retry_wifi > retry_wifi_interval) {

		debug_info(
				get_date_timestamp_str(get_timestamp(milli), Timeoffset) + ": retrying WiFi...");
		wifi_connected = nw_start(STARTUP_WIFI_TIMEOUT,
		STARTUP_WIFI_RETRY_DELAY);
		last_retry_wifi = millis();
		retry_wifi_interval += retry_wifi_interval * RETRY_WIFI_FACTOR;
		wifi_connected = nw_start(STARTUP_WIFI_TIMEOUT,
		STARTUP_WIFI_RETRY_DELAY);
	}
	if (wifi_connected) {

		debug_info(
				get_date_timestamp_str(get_timestamp(millis()), Timeoffset) + ": WiFi connected");

	}

	return (0);

}
/*
 * Check if OM2M is Due
 */

bool is_om2m_pub_due(unsigned long milli) {

	if (millis() - last_om2m_pub > TIME_INTERVAL_OM2M) {
		debug_info("17 Time interval : ");
		debug_info(milli);

		return (true);
	} else
		return (false);
}

/*
 * Check if Thingspeak id Due
 */
bool is_thngspk_pub_due(unsigned long milli) {
	if (millis() - last_thngspk_pub > TIME_INTERVAL_THINGSPEAK) {

		return (true);
	} else
		return (false);
}

void update_hist_idx_om2m(int pub_count) {
	//some data is published, update the hist_idx_om2m
	if (pub_count > 0) {
		//check if hist idx overflows the max buf
		if (hist_idx_om2m + pub_count < PRIMARY_BUF_COUNT) {
			hist_idx_om2m += pub_count;
		} else {
			hist_idx_om2m = hist_idx_om2m + pub_count - PRIMARY_BUF_COUNT;
		}
	}
}

void update_hist_idx_thngspk(int pub_count) {
	if (pub_count > 0) {
		if (hist_idx_thngspk + pub_count < PRIMARY_BUF_COUNT) {
			hist_idx_thngspk += pub_count;
		} else {
			hist_idx_thngspk = hist_idx_thngspk + pub_count - PRIMARY_BUF_COUNT;
		}
	}
}


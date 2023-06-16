#define VERSION "V1.00.16"

#include "WM-FLOW.h"
#include "scrc_constants.h"

#if defined(WM_WF)

#include<Arduino.h>
#include "HardwareSerial.h"

#include "scrc_node.h"
#include "scrc_utils.h"

#include "scrc_flowmeter.h"
#include "scrc_pressuresensor.h"

extern int count_hw;
extern struct sensors_data data_primary[PRIMARY_BUF_COUNT];

extern short int idx_data_primary;

extern short int hist_idx_om2m;

extern short int hist_idx_thngspk;

extern  bool wifi_connected;

/*
 *Timing Variables
 */

extern  uint32_t last_om2m_pub;

extern  uint32_t last_thngspk_pub;

extern  uint32_t last_ms_read_data_primary;

namespace {

const int SERIAL_BAUD_RATE = 115200;

}

namespace {

const int MAIN_LOOP_DELAY = 5000;

}

/*
 * Buffer Information
 */

/*
 * Function Definitions
 */
void read_data_from_hw_FLOW();
/*
 * Setup Function
 */

void setup_flow() {
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.print("***** Setup (from Eclipse ");
	Serial.print(VERSION);
	Serial.println(") *****");
	Serial.println(" ");

	//primary_buf_init(data_primary);

	nw_setup();
	wifi_connected = nw_start(STARTUP_WIFI_TIMEOUT, STARTUP_WIFI_RETRY_DELAY);

	//check if connected
	if (wifi_connected) {
		Serial.println("\n WiFi connected.");

		debug_info("Inside Time");

		//sync ts from ntp server
		sync_time();

		//get local ts
		uint64_t ts = get_timestamp();
		Serial.println(
				"Local timestamp: " + get_timestamp_str(ts, Timeoffset) + "\t"
						+ "Date: " + get_datestamp_str(ts, Timeoffset));

		// get utc ts
		String time_stamp = get_date_timestamp_str(ts, Timeoffset);
		Serial.println("UTC :-: Time :-: Date: " + time_stamp); // For UTC, take timeoffset as 0
		Serial.println("");
	}

	else
		Serial.println("\n WiFi not connected.");

	/*
	 * 	 setup for various sensors connected to hw
	 */
#if !defined(PRESSURE_MODBUS)
	hw_setup_pressuresensor();
#endif

	//hw_setup_oled();

	/*
	 Thing Speak Setup
	 */
	if (wifi_connected) {
		//TODO: when internet is not available, this fn hangs. have a timeout to fix the issue. address all other thingspeak n/w issues
		pub_setup_thingspeak();
	}

	debug_info("***** Endof setup *****");

}

/*
 *The loop function is called in an endless loop
 */
void loop_flow() {


	debug_info("Am inside Loop");

	wifi_connected = nw_is_connected();
	// read data from h/w
	read_data_from_hw_FLOW();

	/*
	 * n/w available and pub to onem2m is due, publish primary data
	 */
	if (wifi_connected && is_om2m_pub_due(millis())) {

		int pub_count = pub_pri_buf_data(data_primary, idx_data_primary,
				hist_idx_om2m, true);

		//some data is published, update the hist_idx_om2m
		update_hist_idx_om2m(pub_count);

		//update the last pub time
		last_om2m_pub = millis();
	}

	/*
	 *n/w available and pub to thngspk is due, publish primary data
	 */
	if (wifi_connected && is_thngspk_pub_due(millis())) {

		int pub_count = pub_thingspeak(data_primary, idx_data_primary,
				hist_idx_thngspk, true);

		update_hist_idx_thngspk(pub_count);

		last_thngspk_pub = millis();
	}

	Serial.println("Trying to connect to wi-fi " + String(MAIN_SSID));
//if(!wifi_connected)
	//retry_wifi();
	nw_start(STARTUP_WIFI_TIMEOUT,
	STARTUP_WIFI_RETRY_DELAY);

	delay(MAIN_LOOP_DELAY);
	delay(30000);
}

/*
 * Read Hardware Data
 */
void read_data_from_hw_FLOW() {

	if (millis() - last_ms_read_data_primary > TIME_INTERVAL_PRIMARY) {
		for (int i = 0; i < NO_OF_NODES; i++) {
			data_primary[idx_data_primary % PRIMARY_BUF_COUNT].node_id=i;
			uint64_t time_stamp = get_timestamp();
			String timestamp = get_date_timestamp_str(time_stamp, 0);

			data_primary[idx_data_primary % PRIMARY_BUF_COUNT].time_stamp =
					time_stamp;

			debug_info(
					get_date_timestamp_str(time_stamp, Timeoffset) + ": read_data_from_hw...\n");

			/*
			 read data from hardware
			 */

			timestamp = get_date_timestamp_str(time_stamp, 0);

			data_primary[idx_data_primary % PRIMARY_BUF_COUNT].time_stamp =
					time_stamp;
			hw_read_flowmeter(
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].flowrate,
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].totalflow,
					i);

			hw_read_pressuresensor(
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].pressure,
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].pressurevoltage,
					i);

			debug_info("\nCount of Hardware outputs:  " + String(count_hw++));

			last_ms_read_data_primary = millis();
			if (idx_data_primary == PRIMARY_BUF_COUNT - 1) {
				idx_data_primary = 0;
			} else
				idx_data_primary++;

			if (!wifi_connected) {
				if (idx_data_primary == hist_idx_om2m)
					update_hist_idx_om2m(1);
				if (idx_data_primary == hist_idx_thngspk)
					update_hist_idx_thngspk(1);
			}
		}
	}
}

#endif

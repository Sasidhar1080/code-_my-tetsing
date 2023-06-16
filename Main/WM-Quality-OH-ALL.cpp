
#define VERSION "V1.00.16"

#include "WM-Quality-OH-ALL.h"
#include "scrc_constants.h"
#if defined(WM_WD_OH_ALL)
#include<Arduino.h>
#include "HardwareSerial.h"

#include "scrc_node.h"
#include "scrc_utils.h"

#include "scrc_dsb182011.h"
#include "scrc_tds.h"
#include "scrc_pH.h"
#include "scrc_turbidity.h"
#include "scrc_level.h"
#include "scrc_oled.h"
#include "scrc_motor.h"


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
void read_data_from_hw_OH_ALL();

/*
 * Setup Function
 */

void setup_oh_all() {
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
	hw_setup_oled();
	hw_setup_tds();
	hw_setup_temp();

hw_setup_pH();
hw_setup_level();
hw_setup_turbidity();


hw_setup_motor_in();
hw_setup_motor_out();

	/*
	 Thing Speak Setup
	 */
	if (wifi_connected) {
		//TODO: when internet is not available, this fn hangs. have a timeout to fix the issue. address all other thingspeak n/w issues
		display_oled(data_primary, idx_data_primary-1);
		pub_setup_thingspeak();
	}

	debug_info("***** Endof setup *****");

}


/*
 *The loop function is called in an endless loop
 */
void loop_oh_all() {


	debug_info("Am inside Loop");


	wifi_connected = nw_is_connected();
	// read data from h/w
	read_data_from_hw_OH_ALL();

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
display_oled(data_primary, idx_data_primary-1);
	delay(MAIN_LOOP_DELAY);
	delay(5000);
}


void read_data_from_hw_OH_ALL() {

	if (millis() - last_ms_read_data_primary > TIME_INTERVAL_PRIMARY) {

		uint64_t time_stamp = get_timestamp();
		String timestamp = get_date_timestamp_str(time_stamp, 0);
		data_primary[idx_data_primary % PRIMARY_BUF_COUNT].time_stamp =
				time_stamp;

		debug_info(
				get_date_timestamp_str(time_stamp, Timeoffset) + ": read_data_from_hw...\n");

		/*
		 read data from hardware
		 */

		 hw_set_motor_rinse(1);
		debug_info("11 idx_data_primary  "+ String(idx_data_primary));
		float temp123;
			hw_read_temp(&temp123);
			int stat = hw_read_temp(
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].temp);
			debug_info(stat);

			hw_read_tds(
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].tdsvoltage,
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].tdswithouttemp,
					&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].tdswithtemp);

			hw_read_level(&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].level);

			hw_read_turbidity(&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].turbidity);
			hw_read_pH(&data_primary[idx_data_primary % PRIMARY_BUF_COUNT].pH);

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

#endif

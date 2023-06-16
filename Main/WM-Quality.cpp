#define VERSION "V1.00.16"

#include <Arduino.h>
#include "stdint.h"

// Do not remove the include below
#include "WM-Quality.h"

#include "scrc_constants.h"
#include "scrc_node.h"
#include "scrc_oled.h"

int count_hw;

struct sensors_data data_primary[PRIMARY_BUF_COUNT];

short int idx_data_primary;

short int hist_idx_om2m;

short int hist_idx_thngspk;

bool wifi_connected;



/*
 *Timing Variables
 */

uint32_t last_om2m_pub;

uint32_t last_thngspk_pub;

uint32_t last_ms_read_data_primary;

uint32_t last_retry_wifi;

uint32_t retry_wifi_interval;

int ssid_mas_size;

void setup() {
	count_hw = 0;

	idx_data_primary = 0;

	hist_idx_om2m = 0;

	hist_idx_thngspk = 0;

	wifi_connected = false;

	/*
	 *Timing Variables
	 */

	last_om2m_pub = 0;

	last_thngspk_pub = 0;

	last_ms_read_data_primary = 0;

	last_retry_wifi = 0;

	retry_wifi_interval = RETRY_WIFI_INTERVAL;


	ssid_mas_size = sizeof(MAIN_SSID) / sizeof(MAIN_SSID[0]);



#if defined(WM_WD_RO)
	setup_ro(); //
#endif

#if defined(WM_WD_OH)
	setup_oh();
#endif

#if defined(WM_WD_OH_ALL)
	setup_oh_all();
#endif

#if defined(WM_WF)
	setup_flow();
#endif
}

/*
 *The loop function is called in an endless loop
 */
void loop() {

#if defined(WM_WD_RO)
	loop_ro();
#endif


#if defined(WM_WD_OH)
	loop_oh();
#endif

#if defined(WM_WD_OH_ALL)
	loop_oh_all();
#endif

#if defined(WM_WF)
	loop_flow();
#endif
}

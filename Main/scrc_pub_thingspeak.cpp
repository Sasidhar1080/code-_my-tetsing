#include "scrc_pub_thingspeak.h"
#include "scrc_constants.h"
#include "scrc_http.h"
#include "scrc_wifi.h"
#include "scrc_time.h"
#include "scrc_node.h"

#include <WiFi.h>
#include <ThingSpeak.h>

#include <Arduino.h>
#include <HardwareSerial.h>

WiFiClient client; // @suppress("Abstract class cannot be instantiated")

int HTTPPost(int numFields, String fieldData[], String time_stamp);

String getResponse();

/*
 * Setup ThingSpeak
 */
void pub_setup_thingspeak() {
	ThingSpeak.begin(client);  // Initialize ThingSpeak
}
/*
 * publish ThingspeaK Data
 */
int pub_thingspeak(const struct sensors_data *ptr_buf,
		const short int latest_buf_idx, const short int hist_buf,
		bool pub_hist) {

	//Local variables
	int pub_count = 0;
	String fieldData[ NUM_FIELDS];
	int status = 0;
	int end_idx;

	//Interval calculation
	unsigned long hw_time_interval = TIME_INTERVAL_PRIMARY;
	unsigned long thngspk_pub_interval = TIME_INTERVAL_THINGSPEAK;
	int max = PRIMARY_BUF_COUNT;

	debug_info("....sizeof(buffer): ");
	debug_info(sizeof(ptr_buf));
	debug_info(", sizeof(typeof(buffer)): ");
	debug_info(sizeof(typeof(ptr_buf[0])));
	debug_info("\n");

	//TODO: dynamically calculate max
	//int max = sizeof(ptr_buf) / sizeof(typeof(ptr_buf[0]));

	debug_info("Start pub primary data...\n");
	debug_info("latest_buf_idx : " + String(latest_buf_idx));
	debug_info("hist_buf : " + String(hist_buf));

	// Calculating Number of records based on hist-idx and  latest-idx
	int no_records;
	if (!pub_hist) {
		no_records = thngspk_pub_interval / hw_time_interval;
	} else {
		if (latest_buf_idx > hist_buf) {
			no_records = latest_buf_idx - hist_buf;
		} else {
			no_records = max - hist_buf + latest_buf_idx;
		}
	}

	// Updating the start and end index
	int start_idx = latest_buf_idx + (max - no_records);

	if (latest_buf_idx == 0)
		end_idx = PRIMARY_BUF_COUNT - 1;
	else
		end_idx = max + latest_buf_idx - 1;

	debug_info("no_records: " + String(no_records) + "\n");
	debug_info("start_idx: " + String(start_idx % max) + "\n");
	debug_info("end_idx: " + String(end_idx % max) + "\n");
	debug_info("max_record_count: " + String(max) + "\n");

	for (int i = start_idx; i <= end_idx; i++) {

		//check if WiFi available
		if (!nw_is_connected()) {
			debug_info("no n/w, do nothing!!!\n");
			status = E_THINGSPEAK_NW;
		}

		int idx = i % max;
#if defined(WM_WD_RO)

		debug_info("Temp Data on ThingSpeak: ");
		debug_info(ptr_buf[idx].temp);
		debug_info("TDS Data on ThingSpeak: ");
		debug_info(ptr_buf[idx].tdswithtemp);


		//Formating data for thingspeak publish

		fieldData[0] = ptr_buf[idx].node_id;
#endif

#if defined(WM_WD_RO)

		fieldData[1] = String(ptr_buf[idx].temp);
		fieldData[2] = String(ptr_buf[idx].tdsvoltage);
		fieldData[3] = String(ptr_buf[idx].tdswithouttemp);
		fieldData[4] = String(ptr_buf[idx].tdswithtemp);



#endif

#if defined(WM_WD_OH_ALL)

		fieldData[1] = String(ptr_buf[idx].temp);
		fieldData[2] = String(ptr_buf[idx].tdsvoltage);
		fieldData[3] = String(ptr_buf[idx].tdswithouttemp);
		fieldData[4] = String(ptr_buf[idx].tdswithtemp);
		fieldData[5] = String(ptr_buf[idx].level);
		fieldData[6] = String(ptr_buf[idx].turbidity);
		fieldData[7] = String(ptr_buf[idx].pH);
#endif

#if defined(WM_WD_OH)

		fieldData[1] = String(ptr_buf[idx].level);
		fieldData[2] = String(ptr_buf[idx].turbidity);
		fieldData[3] = String(ptr_buf[idx].pH);
#endif

#if defined(WM_WF)

		fieldData[1] = String(ptr_buf[idx].flowrate);
		fieldData[2] = String(ptr_buf[idx].totalflow);
		fieldData[3] = String(ptr_buf[idx].pressure);
		fieldData[4] = String(ptr_buf[idx].pressurevoltage);
#endif

		String timestamp = get_date_timestamp_str(ptr_buf[idx].time_stamp, 0);

		//Calling the post function
		status = HTTPPost( NUM_FIELDS, fieldData, timestamp);

		// updating and returning the number of successful data publishes
		if (status == SUCCESS_THINGSPEAK) {
			pub_count++;
			delay(200);
		} else if (status == E_THINGSPEAK_NW) {
			break;
		} else if (status == E_THINGSPEAK_CONNECT) {
			//Thingspeak connectivity error Handling
			break;
		} else if (status == E_THINGSPEAK_CONNECTION) {

			//Thingspeak no response error Handling
			break;
		} else if (status == E_THINGSPEAK_NO_RESPONSE) {

			//Thingspeak empty response error Handling
			break;
		} else if (status == E_THINGSPEAK_EMPTY_RESPONSE) {
			break;
		}

	}
	return pub_count;
}

/*
 * Post Function
 */
int HTTPPost(int numFields, String fieldData[], String time_stamp) {

	if (client.connect( THING_SPEAK_ADDRESS, 80)) {
		String apikey;
		int node_id = fieldData[0].toInt();
		if (node_id == 0)
			apikey = String() + WRITEAPIKEY_0;
#if defined(WM_WF)
		if (node_id == 1)
			apikey = String() + WRITEAPIKEY_1;
		if (node_id == 2)
			apikey = String() + WRITEAPIKEY_2;
		if (node_id == 3)
			apikey = String() + WRITEAPIKEY_3;
#endif

		// Build the postData string.
		// If you have multiple fields, make sure the sting does not exceed 1440 characters.
		String postData = "api_key=" + apikey + "&" + "created_at="
				+ time_stamp;
		for (int fieldNumber = 1; fieldNumber < numFields + 1; fieldNumber++) {
			String fieldName = "field" + String(fieldNumber);
			postData += "&" + fieldName + "=" + fieldData[fieldNumber];
		}

		// POST data via HTTP.
		debug_info("Connecting to ThingSpeak for update...");
		debug_info();

		//Posting the data to thingspeak
		client.println("POST /update HTTP/1.1");
		client.println("Host: api.thingspeak.com");
		client.println("Connection: close");
		client.println("Content-Type: application/x-www-form-urlencoded");
		client.println("Content-Length: " + String(postData.length()));
		client.println();
		client.println(postData);

		debug_info(postData);

		//Printing the response
		String answer = getResponse();
		debug_info(answer);
	} else {

		debug_info("Connection Failed");
		return E_THINGSPEAK_CONNECT;
	}

	return SUCCESS_THINGSPEAK;
}

/*
 *Response Handling Function
 */

String getResponse() {
	String response;
	long startTime = millis();

	delay(200);
	while (client.available() < 1 && ((millis() - startTime) < TIMEOUT_NETWORK)) {
		delay(5);
	}

	if (client.available() > 0) { // Get response from server.
		char charIn;
		do {
			charIn = client.read(); // Read a char from the buffer.
			response += charIn;     // Append the char to the string response.
		} while (client.available() > 0);
	}
	client.stop();

	return response;
}

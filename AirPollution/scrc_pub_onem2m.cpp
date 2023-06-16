/*
   scrc_pub_onem2m.cpp

    Created on: 12-May-2022
        Author: Ayu Parmar
*/

#include <Arduino.h>

#include "scrc_constants.h"
#include "scrc_pub_onem2m.h"
#include "scrc_http.h"
#include "scrc_wifi.h"
#include "scrc_time.h"


int publish_onem2m(const struct sensors_data *ptr_sensors_data,
                   const bool is15sec);

/*
   Publish 15 sec data

*/
int count_15 = 1;
int count_10 = 1;

int pub_15_sec_data(const struct sensors_data *ptr_buf,
                    const short int latest_buf_idx, const short int hist_buf_idx, bool pub_hist) {

  int status = 0;
  int hw_time_interval = TIME_INTERVAL_15_SEC;
  unsigned long om2m_pub_interval = TIME_INTERVAL_1_MIN;

  debug_info("....sizeof(buffer): ");
  debug_info(sizeof(ptr_buf));
  debug_info(", sizeof(typeof(buffer)): ");
  debug_info(sizeof(typeof(ptr_buf[0])));
  debug_info("\n");

  int max = PRIMARY_BUF_COUNT;
  //TODO: dynamically calculate max
  //int max = sizeof(ptr_buf) / sizeof(typeof(ptr_buf[0]));

  debug_info("Start pub 15 sec data...\n");

  //check if WiFi available
  if (!nw_is_connected()) {
    debug_info("no n/w, do nothing!!!\n");
    return -1;
  }

  int no_records = (om2m_pub_interval / hw_time_interval) - 1;

  /*  if (no_records != count_hw)
    {
      debug_info("entered in count for loop");
      no_records = count_hw;
    }*/


  int start_idx = latest_buf_idx + (max - no_records) + 1;
  int end_idx = max + latest_buf_idx;
  debug_info("no_records: " + String(no_records) + "\n");
  debug_info("start_idx: " + String(start_idx) + "\n");
  debug_info("end_idx: " + String(end_idx) + "\n");
  debug_info("max_record_count: " + String(max) + "\n");

  /*if (!connect_http()) {
  	debug_info("connecting onem2m failed!!!\n");
  	return -2;
    }*/

  debug_info("before for loop");
  for (int i = start_idx; i <= end_idx; i++) {
    debug_info("inside for loop");


    if (!connect_http()) {
      debug_info("connecting onem2m failed!!!\n");
      return -2;
    }

    Serial.print("Temp debug 1: ");
    float temp = ptr_buf[i % max].temp;
    Serial.println(ptr_buf[i % max].temp);

    status = publish_onem2m(&ptr_buf[i % max], true);

    Serial.print("count of 15 sec data: ");
    Serial.println(count_15++);

  }

  return status;
}

/*
   Publish 10 min data

*/
int pub_10_min_data(const struct sensors_data *ptr_buf, int hist_start_idx,
                    const short int latest_buf_idx) {

  int status = 0;
  int hw_time_interval = TIME_INTERVAL_10_MIN;
  unsigned long om2m_pub_interval = TIME_INTERVAL_10_MIN;

  debug_info("....sizeof(buffer): ");
  debug_info(sizeof(ptr_buf));
  debug_info(", sizeof(typeof(buffer)): ");
  debug_info(sizeof(typeof(ptr_buf[0])));
  debug_info("\n");

  int max = SEC_BUF_COUNT;

  debug_info("Start pub 10 min data...\n");

  //check if WiFi available
  if (!nw_is_connected()) {
    debug_info("no n/w, do nothing!!!\n");
    return -1;
  }

  int no_records = latest_buf_idx - hist_start_idx ;

  int start_idx = latest_buf_idx + (max - no_records);
  int end_idx = max + latest_buf_idx - 1;
  debug_info("no_records: " + String(no_records) + "\n");
  debug_info("start_idx: " + String(start_idx) + "\n");
  debug_info("end_idx: " + String(end_idx) + "\n");
  debug_info("max_record_count: " + String(max) + "\n");
  debug_info("before for 10 min loop");

  for (int i = start_idx; i <= end_idx; i++) {

    if (!connect_http()) {
      debug_info("connecting onem2m failed!!!\n");
      return -2;
    }

    debug_info("........i: " + String(i) + "\n");

    status = publish_onem2m(&ptr_buf[i % max], false);

    Serial.print("count of 10 min data: ");
    Serial.println(count_10++);
  }
  return status;
}

/*
   Pub to onem2m_common

*/
int publish_onem2m(const struct sensors_data *ptr_sensors_data,
                   const bool is15sec) {
  char data[MAX_STRING_LEN];
  debug_info("AQI: ");
  debug_info(ptr_sensors_data->aqi);

  debug_info("AQL: ");
  debug_info(ptr_sensors_data->aql);

  sprintf(data,
          "[%ld, %.2f, %.2f, %.2f, %.2f,%d]",
          ptr_sensors_data->time_stamp,
          ptr_sensors_data->pm25, ptr_sensors_data->pm10,
          ptr_sensors_data->temp, ptr_sensors_data->rh,
          ptr_sensors_data->aqlmp,is15sec ? 0 : 1);  // 0 --> data from primary buffer
  // 1 --> data from secondary 

  debug_info(data);
  debug_info("\n");

  //String req_data = String() + "{\"m2m:cin\":{\"con\":\"" + data + "\"}}";
      String req_data = String() + "{\"m2m:cin\": {"

                                 + "\"con\": \"" + data + "\","

                                 + "\"lbl\": " + OM2M_DATA_LBL + ","

                                 + "\"cnf\": \"text\""

                                 +"}}";
  Serial.println(req_data);

  String link = String() + OM2M_MN + OM2M_AE + "/" + OM2M_DATA_CONT;
  Serial.print("onem2m ae/mn/node: ");
  Serial.println(link);
  String req = String() + "POST " + link + " HTTP/1.1\r\n" +
               "Host: " + CSE_IP + "\r\n" +
               "X-M2M-Origin:" + OM2M_ORGIN + "\r\n" +
               "Content-Type:application/json;ty=" + 4 + "\r\n" +
               "Content-Length: " + req_data.length() + "\r\n" +
               "Connection: close\r\n\n" +
               req_data;
  return post_request(req);
}

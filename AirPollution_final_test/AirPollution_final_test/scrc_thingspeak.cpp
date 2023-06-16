/*
   scrc_thingspeak.cpp

    Created on: 16-Oct-2020
        Author: BSRC-Sam
*/

#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

#include "scrc_thingspeak.h"
#include "scrc_constants.h"
#include "scrc_http.h"
#include "scrc_wifi.h"
#include "scrc_time.h"

#define TIMEOUT  5000 
#define NUM_FIELDS 8
#define THING_SPEAK_ADDRESS "api.thingspeak.com"
String writeAPIKey = "PSKWJ8GP0HS55WL6";        

// unsigned long myChannelNumber = 939888; //IIITH main gate
// const char *myWriteAPIKey = "WDVVTDSX00XL5V05";

// unsigned long myChannelNumber = 1099442; //Pump House
// const char *myWriteAPIKey = "UUESWH1NUKNQXM47";

// unsigned long myChannelNumber = 944798; //Faculty quarters
// const char *myWriteAPIKey = "IGN8L3ACV8PWB9YK";
////
// unsigned long myChannelNumber = 885762; //Guest house
// const char *myWriteAPIKey = "VWMXNW6NJOQVVVI3";

// unsigned long myChannelNumber = 937750; //Felicity ground 
// const char *myWriteAPIKey = "W9L6N34IC1MNE7HN";

// unsigned long myChannelNumber = 938513; //Palash nivas
// const char *myWriteAPIKey = "SP2M6BRIC92GPIM8";

// unsigned long myChannelNumber = 947311; //Behind KRB 
// const char *myWriteAPIKey = "1CLG8H9WWR7Y7HGF";

// unsigned long myChannelNumber = 944545; //Library
// const char *myWriteAPIKey = "LH08QHJZ82YEXWUL";

// unsigned long myChannelNumber = 945028; //Football ground 
// const char *myWriteAPIKey = "4PCMYWJ8XUHUSHUW";

unsigned long myChannelNumber = 928486; //Bakul nivas
const char *myWriteAPIKey = "PSKWJ8GP0HS55WL6";
WiFiClient client;

void pub_setup_thingspeak() {
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

int pub_thingspeak(const sensors_data *ptr_buf,
                   const short int latest_buf_idx) {

  String fieldData[ NUM_FIELDS ];
  int status = 0;
  int hw_time_interval = TIME_INTERVAL_15_SEC;
  unsigned long om2m_pub_interval = TIME_INTERVAL_1_MIN;

  debug_info("....sizeof(buffer): ");
  debug_info(sizeof(ptr_buf));
  debug_info(", sizeof(typeof(buffer)): ");
  debug_info(sizeof(typeof(ptr_buf[0])));
  debug_info("\n");

  int max = PRIMARY_BUF_COUNT;
  debug_info("Start pub 15 sec data...\n");

  //check if WiFi available
  if (!nw_is_connected()) {
    debug_info("no n/w, do nothing!!!\n");
    return -1;
  }

  int no_records = om2m_pub_interval / hw_time_interval;

  int start_idx = latest_buf_idx + (max - no_records) + 1;
  int end_idx = max + latest_buf_idx;
  debug_info("no_records: " + String(no_records) + "\n");
  debug_info("start_idx: " + String(start_idx) + "\n");
  debug_info("end_idx: " + String(end_idx) + "\n");
  debug_info("max_record_count: " + String(max) + "\n");
  debug_info("before for loop");
  for (int i = start_idx; i <= end_idx; i++) {
    debug_info("inside for loop");

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].temp);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].rh);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].pm25);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].pm10);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].temp);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].CO2);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].aqi);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].noise);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].co);

      Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].no2);

    fieldData[1] = String(ptr_buf[i % max].temp);
    fieldData[2] = String(ptr_buf[i % max].rh);
    fieldData[3] = String(ptr_buf[i % max].pm25);
    fieldData[4] = String(ptr_buf[i % max].pm10);
    fieldData[5] = String(ptr_buf[i % max].co);
    fieldData[6] = String(ptr_buf[i % max].no2);
    fieldData[7] = String(ptr_buf[i % max].nh3);
    //fieldData[8] = String(ptr_buf[i % max].nh3);
    String timestamp = get_date_timestamp_str(ptr_buf[i % max].time_stamp, 0);

    HTTPPost( NUM_FIELDS , fieldData, i, timestamp);
    delay(200);
  }
  return 0;
}



int pub_10_min_thingspeak(const struct sensors_data *ptr_buf, int hist_start_idx,
                    const short int latest_buf_idx) {

  String fieldData[ NUM_FIELDS ];
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
    debug_info("inside for loop");

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].temp);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].rh);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].pm25);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].pm10);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].temp);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].CO2);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].aqi);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].noise);

    Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].co);

      Serial.println("Data on ThingSpeak: ");
    Serial.print(ptr_buf[i % max].no2);

    fieldData[1] = String(ptr_buf[i % max].temp);
    fieldData[2] = String(ptr_buf[i % max].rh);
    fieldData[3] = String(ptr_buf[i % max].pm25);
    fieldData[4] = String(ptr_buf[i % max].pm10);
    fieldData[5] = String(ptr_buf[i % max].co);
    fieldData[6] = String(ptr_buf[i % max].no2);
    fieldData[8] = String(ptr_buf[i % max].nh3);
    fieldData[7] = String(1);
    
    
    String timestamp = get_date_timestamp_str(ptr_buf[i % max].time_stamp, 0);

    HTTPPost( NUM_FIELDS , fieldData, i, timestamp);
    delay(200);
  }
  return 0;
}


int HTTPPost( int numFields , String fieldData[], int i, String time_stamp  ) {

  if (client.connect( THING_SPEAK_ADDRESS , 80 )) {

    // Build the postData string.
    // If you have multiple fields, make sure the sting does not exceed 1440 characters.
    String postData = "api_key=" + writeAPIKey + "&" + "created_at=" + time_stamp;
    for ( int fieldNumber = 1; fieldNumber < numFields + 1; fieldNumber++ ) {
      String fieldName = "field" + String( fieldNumber );
      postData += "&" + fieldName + "=" + fieldData[ fieldNumber ];
    }

    // POST data via HTTP.
    Serial.println( "Connecting to ThingSpeak for update..." );
    Serial.println();

    client.println( "POST /update HTTP/1.1" );
    client.println( "Host: api.thingspeak.com" );
    client.println( "Connection: close" );
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println( "Content-Length: " + String( postData.length() ) );
    client.println();
    client.println( postData );

    Serial.println( postData );

    String answer = getResponse();
    Serial.println( answer );
  }
  else
  {
    Serial.println ( "Connection Failed" );
  }
return 0;
}

// Wait for a response from the server indicating availability,
// and then collect the response and build it into a string.

String getResponse() {
  String response;
  long startTime = millis();

  delay( 200 );
  while ( client.available() < 1 && (( millis() - startTime ) < TIMEOUT ) ) {
    delay( 5 );
  }

  if ( client.available() > 0 ) { // Get response from server.
    char charIn;
    do {
      charIn = client.read(); // Read a char from the buffer.
      response += charIn;     // Append the char to the string response.
    } while ( client.available() > 0 );
  }
  client.stop();

  return response;
}

/*
 * scrc_http.cpp
 *
 *  Created on: 12-May-2022
 *      Author: Ayu Parmar
 */

#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "scrc_http.h"
#include "scrc_wifi.h"
#include "scrc_constants.h"

WiFiClientSecure httpsClient; // @suppress("Abstract class cannot be instantiated")

void connect_setup_http() {

}

bool connect_http() {

  Serial.print("HTTPS Connecting: https://");
  Serial.print(CSE_IP);
  Serial.print(":");
  Serial.print(CSE_PORT);
  Serial.print("/");
  Serial.printf("; Using finger print '%s'\n", FINGER_PRINT);

  httpsClient.setFingerprint(FINGER_PRINT);
  httpsClient.setTimeout(2000);                                  // 2 Seconds
  //TODO: enable keep alive???
  delay(1000);

  int r = 0;                                                   //retry counter
  while ((!httpsClient.connect(CSE_IP, CSE_PORT)) && (r < 30)) {
    delay(1000);
    Serial.print(".");
    r++;
  }

  Serial.print("requesting URL: ");
  Serial.println(CSE_IP);

  if (r == 30) {
    Serial.println(": Connection failed");
    // return error
    return false;
  } else {
    Serial.println(": Connected to web");
    //return success
    return true;
  }
}

int post_request(String req) {
  int status = 0;

  //POST request
  status = httpsClient.print(req);

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  String line;
  while (httpsClient.available()) {
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("closing connection");

  return status;
}

void disconnect_http() {
  Serial.print("HTTPS disconnected.");
  httpsClient.stop();
}

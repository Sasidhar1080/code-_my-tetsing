/*
 * scrc_http.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */

#ifndef SCRC_HTTP_H_
#define SCRC_HTTP_H_

//#define DEBUG False

#include <WString.h>
#include "Arduino.h"
#include "HardwareSerial.h"
bool connect_http();
void disconnect_http();
int post_request(String req);

#endif /* SCRC_HTTP_H_ */

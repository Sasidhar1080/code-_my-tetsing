/*
 * scrc_http.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Ganesh CGS
 */

#ifndef SCRC_HTTP_H_
#define SCRC_HTTP_H_

//#define DEBUG False

#include <WString.h>

bool connect_http();
void disconnect_http();
int post_request(String data);

#endif /* SCRC_HTTP_H_ */

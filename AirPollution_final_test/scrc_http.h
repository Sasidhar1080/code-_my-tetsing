/*
 * scrc_http.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */

#ifndef INCLUDE_SCRC_HTTP_H_
#define INCLUDE_SCRC_HTTP_H_

void connect_setup_http();
bool connect_http();
void disconnect_http();

int post_request(String req);

#endif /* INCLUDE_SCRC_HTTP_H_ */

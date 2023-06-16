/*
 * scrc_http.cpp
 *
 *  Created on: Feb 23, 2020
 *      Author: Usha Sai
 */
/*
 * scrc_http.ino
 *
 *  Created on: Nov 9, 2020
 *      Author: suhas
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "scrc_constants.h"
#include "scrc_http.h"
#include "scrc_wifi.h"
const char *ca_cert = "-----BEGIN CERTIFICATE-----\n" \
		"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n" \
		"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n" \
		"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n" \
		"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n" \
		"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n" \
		"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n" \
		"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n" \
		"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n" \
		"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n" \
		"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n" \
		"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n" \
		"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n" \
		"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n" \
		"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n" \
		"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n" \
		"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n" \
		"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n" \
		"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n" \
		"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n" \
		"-----END CERTIFICATE-----\n";
#if(HTTPS)
WiFiClientSecure Client; // @suppress("Abstract class cannot be instantiated")
#else
WiFiClient Client; // @suppress("Abstract class cannot be instantiated")
#endif
bool connect_http() {

	debug_info("HTTP Connecting: http://");
	debug_info(CSE_IP);
	debug_info(":");
	debug_info(CSE_PORT);
	debug_info("/");
#if(HTTPS)
		Client.setCACert(ca_cert);
#endif

	int r = 0;                                                   //retry counter
	while ((!Client.connect(CSE_IP, CSE_PORT)) && (r < 30)) {
		delay(1000);
		Serial.print(".");
		r++;
	}

	debug_info("requesting URL: ");
	debug_info(CSE_IP);

	if (r == 30) {
		debug_info(": Connection failed");
		// return error
		return false;
	} else {
		debug_info(": Connected to web");
		//return success
		//return true;
	}
}

int post_request(String req) {
	int status = 0;
	status = Client.print(req);
	Serial.println("request sent");

	while (Client.connected()) {
		String line = Client.readStringUntil('\n');
		if (line == "\r") {
			Serial.println("headers received");
			break;
		}
	}
	Serial.println("reply was:");
	Serial.println("==========");
	int z=0;
	String line;
	while (Client.available()) {
		line = Client.readStringUntil('\n');  //Read Line by Line
		Serial.println(line); //Print response

		if(z<=0)
			{
			z=line.indexOf("2001");
			}

	}
	Serial.println("==========");
	Serial.println("closing connection");
	Client.flush();
	Client.stop();
	if(z>0)
	{
		return SUCCESS_OM2M;
	}
	else{
		return -101;
	}
	//debug_info("status" + status + "\n");
	Serial.println(status);
	Serial.println(status);
	Serial.println(status);
	Serial.println(status);
	Serial.println(status);
	Serial.println(status);
	delay(3000);
	return status;

}

void disconnect_http() {
	Serial.print("HTTPS disconnected.");
	Client.stop();
}

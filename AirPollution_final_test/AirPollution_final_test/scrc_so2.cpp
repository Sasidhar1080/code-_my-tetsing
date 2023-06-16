/*
 * scrc_so2.cpp
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */
#include <Arduino.h>

#include "scrc_so2.h"

void hw_setup_so2() {

}
void hw_read_so2(float *buf_so2) {
	*buf_so2 = random(1, 10);
}

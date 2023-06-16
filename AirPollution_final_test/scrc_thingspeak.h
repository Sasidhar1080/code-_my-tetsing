/*
   scrc_thingspeak.h

    Created on: 16-Oct-2020
        Author: BSRC-Sam
*/

#include "scrc_constants.h"
#include "scrc_node.h"

#ifndef INCLUDE_SCRC_THINGSPEAK_H_
#define INCLUDE_SCRC_THINGSPEAK_H_

void pub_setup_thingspeak();
int pub_thingspeak(const sensors_data *ptr_buf,
                   const short int latest_buf_idx);
int pub_10_min_thingspeak(const struct sensors_data *ptr_buf, int hist_start_idx,
                    const short int latest_buf_idx);
int HTTPPost( int numFields , String fieldData[], int i, String time_stamp  );
String getResponse();

#endif /* INCLUDE_SCRC_THINGSPEAK_H_ */

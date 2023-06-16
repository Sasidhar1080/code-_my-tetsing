/*
 * scrc_aql.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 */

#ifndef INCLUDE_SCRC_AQL_H_
#define INCLUDE_SCRC_AQL_H_

enum levels {
	Good, Satisfactory, Moderate, Poor, Very_Poor, Severe, Unknown
};
enum aq {
	p25, p10
};

short int aql(float pm);
short int aqi_mp(float pm25, float pm10);
float compute_index_aqi(float p25, float p10);

#endif /* INCLUDE_SCRC_AQL_H_ */

#ifndef __SHT21_H__
#define __SHT21_H__                 			1
    
    #define ADDR_SHT21              			0x40
    
    #include "freertos/FreeRTOS.h"
	#include "freertos/event_groups.h"
	#include "boards.h"
	#include "appconfig.h"

    
    /* Register Map Definitions*/
    #define SHT21_CMD_TEMP_WAIT     			0xE3
    #define SHT21_CMD_TEMP_NO_WAIT  			0xF3
    #define SHT21_CMD_RH_WAIT       			0xE5
    #define SHT21_CMD_RH_NO_WAIT    			0xF5

    /* Register bit-field definitions */
    
    /* Function Declarations */
	void SHT21GetRHInt(uint16_t *rh);
	void SHT21GetTempInt(uint16_t *temp);
	void SHT21GetRHFlt(float *rh);
	void SHT21GetTempFlt(float *temp);
	float SHT21RhConvIntToFlt(uint16_t rhInt);
	float SHT21TempConvIntToFlt(uint16_t tempInt);

#endif

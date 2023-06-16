#ifndef __BOARDS_H__
#define __BOARDS_H__
#include "sdkconfig.h"

#if (CONFIG_CURRENT_BOARD_VERSION == 0)
	#include "ir_gateway_v1.h"
#else
	#error "Board is not defined"
#endif

#endif

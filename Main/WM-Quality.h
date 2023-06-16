// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _WaterMonitoring_H_
#define _WaterMonitoring_H_

#include "scrc_constants.h"

#if defined(WM_WD_RO)
#include "WM-Quality-RO.h"
#endif

#if defined(WM_WD_OH)
#include "WM-Quality-OH.h"
#endif

#if defined(WM_WD_OH_ALL)

#include "WM-Quality-OH-ALL.h"

#endif

#if defined(WM_WF)

#include "WM-FLOW.h"

#endif
//end of add your includes here

//add your function definitions for the project WaterMonitoring here

//Do not add code below this line
#endif /* _WaterMonitoring_H_ */

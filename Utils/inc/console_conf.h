#ifndef INC_CONSOLE_CONF_H_
#define INC_CONSOLE_CONF_H_

#include "../../HW_BSP/inc/vcom.h"

#define OutputInit	vcom_Init
#define OutputTrace	vcom_Trace

#define VERBOSE_LEVEL_0 0
#define VERBOSE_LEVEL_1 1
#define VERBOSE_LEVEL_2 2

#define VERBOSE_LEVEL 0

#if ( VERBOSE_LEVEL < VERBOSE_LEVEL_2)
#define DBG_TRACE_MSG_QUEUE_SIZE 256
#else
#define DBG_TRACE_MSG_QUEUE_SIZE 512
#endif


#endif /* INC_CONSOLE_CONF_H_ */

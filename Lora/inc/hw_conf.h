#ifndef INC_HW_CONF_H_
#define INC_HW_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "stm32f1xx_hw_conf.h"

/* --------Preprocessor compile swicth------------ */
/* debug swicth in debug.h */
//#define DEBUG

/* uncomment below line to never enter lowpower modes in main.c*/
//#define LOW_POWER_DISABLE

/* debug swicthes in bsp.c */
//#define SENSOR_ENABLED




/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* INC_HW_CONF_H_ */

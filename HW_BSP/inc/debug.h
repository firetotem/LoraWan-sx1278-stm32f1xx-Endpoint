

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/



#include <string.h>
#include <stdio.h>
#include "../../HW_BSP/inc/hw_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void DBG_Init( void );

void Error_Handler( void );

#ifdef DEBUG

#define DBG_GPIO_WRITE( gpio, n, x )  HAL_GPIO_WritePin( gpio, n, (GPIO_PinState)(x) )

#define DBG_GPIO_SET( gpio, n )       gpio->BSRR = n

#define DBG_GPIO_RST( gpio, n )       gpio->BRR = n

#define DBG_RTC_OUTPUT RTC_OUTPUT_DISABLE; /* RTC_OUTPUT_ALARMA on PC13 */

#define DBG( x )  do{ x } while(0)

#else /* DEBUG */

#define DBG_GPIO_WRITE( gpio, n, x )

#define DBG_GPIO_SET( gpio, n )

#define DBG_GPIO_RST( gpio, n )

#define DBG( x ) do{  } while(0)

//TODO#define DBG_RTC_OUTPUT RTC_OUTPUT_DISABLE;

#endif /* DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* INC_DEBUG_H_ */

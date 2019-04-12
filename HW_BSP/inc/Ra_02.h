#ifndef INC_RA_02_H_
#define INC_RA_02_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sx1276.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern SX1276_t SX1276;


#define BOARD_WAKEUP_TIME  0 // no TCXO


void SX1276IoInit( void );
void SX1276IoDeInit( void );
bool SX1276CheckRfFrequency( uint32_t frequency );


#ifdef __cplusplus
}
#endif

#endif /* INC_RA_02_H_ */

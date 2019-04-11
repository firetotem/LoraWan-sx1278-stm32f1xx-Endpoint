#ifndef INC_VCOM_H_
#define INC_VCOM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/**
* @brief  init vcom
* @param  callback when Tx buffer has been sent
* @return None
*/
void vcom_Init(  void (*Txcb)(void) );

/**
* @brief  send buffer @p_data of size size to vcom in dma mode
* @param  p_data data to be sent
* @param  szie of buffer p_data to be sent
* @return None
*/
void vcom_Trace(  uint8_t *p_data, uint16_t size );

   /**
* @brief  DeInit the VCOM.
* @param  None
* @return None
*/
void vcom_DeInit(void);

   /**
* @brief  Init the VCOM IOs.
* @param  None
* @return None
*/
void vcom_IoInit(void);

/**
* @brief  DeInit the VCOM IOs.
* @param  None
* @return None
*/
void vcom_IoDeInit(void);

/**
* @brief  last byte has been sent on the uart line
* @param  None
* @return None
*/
void vcom_IRQHandler(void);

/**
* @brief  last byte has been sent from memeory to uart data register
* @param  None
* @return None
*/
void vcom_DMA_TX_IRQHandler(void);

#ifdef __cplusplus
}
#endif


#endif /* INC_VCOM_H_ */
